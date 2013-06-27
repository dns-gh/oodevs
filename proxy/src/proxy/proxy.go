package main

import (
	"flag"
	"fmt"
	"log"
	"net"
	"net/http"
	"net/http/httputil"
	"net/url"
	"runtime"
	"strings"
	"sync"
	"time"
)

const (
	timeout = 1 * time.Hour
)

func main() {
	log.Println("Sword Proxy - copyright Masa Group 2013")

	runtime.GOMAXPROCS(runtime.NumCPU())

	port := flag.Int("port", 8080, "listening port")
	ssl := flag.Bool("ssl", false, "enable SSL")
	certificate := flag.String("ssl_certificate", "", "SSL certificate")
	key := flag.String("ssl_key", "", "SSL key")
	flag.Parse()

	log.Println("port", *port)
	log.Println("ssl", *ssl)
	if *ssl {
		log.Println("certificate", *certificate)
		log.Println("key", *key)
	}

	ctx := NewProxyServer()
	server := &http.Server{
		Addr:           fmt.Sprintf(":%d", *port),
		Handler:        ctx,
		ReadTimeout:    timeout,
		WriteTimeout:   timeout,
		MaxHeaderBytes: http.DefaultMaxHeaderBytes,
	}

	var err error
	if *ssl {
		if len(*certificate) == 0 {
			log.Fatal("Missing SSL certificate")
		}
		if len(*key) == 0 {
			log.Fatal("Missing SSL key")
		}
		err = server.ListenAndServeTLS(*certificate, *key)
	} else {
		err = server.ListenAndServe()
	}
	if err != nil {
		log.Fatal(err)
	}
}

type ProxyContext struct {
	proxy  *httputil.ReverseProxy
	prefix string
	host   string
	port   string
}

type Server struct {
	http.Handler
	access  sync.RWMutex
	targets map[string]*ProxyContext
	local   []net.Addr
}

func NewProxyServer() *Server {
	it := &Server{}
	it.targets = make(map[string]*ProxyContext)
	addr, err := net.InterfaceAddrs()
	if err != nil {
		log.Fatal(err)
	}
	it.local = addr
	return it
}

func (it *Server) FindProxy(url string) *ProxyContext {
	var rpy *ProxyContext
	max := 0
	exact := GetPrefix(url)
	it.access.RLock()
	defer it.access.RUnlock()
	for k, v := range it.targets {
		if exact == k {
			return v
		} else if strings.HasPrefix(url, k) {
			if c := strings.Count(k, "/"); c > max {
				max = c
				rpy = v
			}
		}
	}
	return rpy
}

func (it *Server) ServeHTTP(w http.ResponseWriter, r *http.Request) {

	q, err := url.ParseQuery(r.URL.RawQuery)
	if err != nil {
		http.Error(w, err.Error(), 400)
		return
	}

	if r.URL.Path == "/register_proxy" {
		err = it.Register(q)
		if err != nil {
			http.Error(w, err.Error(), 500)
		}
		return
	} else if r.URL.Path == "/unregister_proxy" {
		it.Unregister(q)
		return
	} else if r.URL.Path == "/list_proxies" {
		it.List(w)
		return
	}

	ctx := it.FindProxy(r.URL.Path)
	if ctx == nil {
		http.NotFound(w, r)
		return
	}
	// url path is a target prefix without trailing slash
	if len(r.URL.Path) < len(ctx.prefix) {
		http.Redirect(w, r, ctx.prefix, http.StatusMovedPermanently)
		return
	}

	ip := it.SetRemoteAddress(r)
	// prev := r.URL.Path
	r.URL.Path = r.URL.Path[len(ctx.prefix):]
	// log.Println(prev, "-> http://"+ctx.host+":"+ctx.port+"/"+r.URL.Path, "from", ip)
	r.Header.Set("Remote-Address", ip)
	ctx.proxy.ServeHTTP(w, r)
}

func (it *Server) SetRemoteAddress(r *http.Request) string {
	ip, _, err := net.SplitHostPort(r.RemoteAddr)
	if err != nil {
		ip = r.RemoteAddr
	}
	for _, v := range it.local {
		if ip == v.String() {
			return it.local[0].String()
		}
	}
	return ip
}

func GetPrefix(prefix string) string {
	if len(prefix) == 0 {
		return "/"
	}
	if !strings.HasPrefix(prefix, "/") {
		prefix = "/" + prefix
	}
	if !strings.HasSuffix(prefix, "/") {
		prefix += "/"
	}
	return prefix
}

func (it *Server) Register(q url.Values) error {
	host := q.Get("host")
	port := q.Get("port")
	url, err := url.Parse("http://" + host + ":" + port + "/")
	if err != nil {
		return err
	}
	prefix := GetPrefix(q.Get("prefix"))
	it.access.Lock()
	defer it.access.Unlock()
	it.targets[prefix] = &ProxyContext{
		proxy:  httputil.NewSingleHostReverseProxy(url),
		prefix: prefix,
		host:   host,
		port:   port,
	}
	log.Println("added proxy", prefix, "to", url)
	return nil
}

func (it *Server) Unregister(q url.Values) {
	prefix := GetPrefix(q.Get("prefix"))
	it.access.Lock()
	delete(it.targets, prefix)
	it.access.Unlock()
	log.Println("removed proxy", prefix)
}

func (it *Server) List(w http.ResponseWriter) {
	it.access.RLock()
	defer it.access.RUnlock()
	fmt.Fprint(w, "[")
	next := false
	for _, v := range it.targets {
		if next {
			fmt.Fprintf(w, ",")
		}
		next = true
		fmt.Fprintf(w, "{\"prefix\":\"%s\",\"host\":\"%s\",\"port\":\"%s\"}", v.prefix, v.host, v.port)
	}
	fmt.Fprint(w, "]")
}
