package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net"
	"net/http"
	"net/http/httputil"
	"net/url"
	"strings"
	"sync"
	"time"
)

const (
	ReadTimeout  = 1 * time.Hour
	WriteTimeout = 1 * time.Hour
)

type ProxyContext struct {
	proxy  *httputil.ReverseProxy
	Prefix string
	Host   string
	Port   string
}

type HttpProxy struct {
	http.Handler
	verbose int
	access  sync.RWMutex
	targets map[string]*ProxyContext
	local   []net.Addr
}

func NewHttpProxy() *HttpProxy {
	it := &HttpProxy{}
	it.targets = make(map[string]*ProxyContext)
	addr, err := net.InterfaceAddrs()
	if err != nil {
		log.Fatal(err)
	}
	it.local = addr
	return it
}

func (s *HttpProxy) Run(options *Options) error {
	s.verbose = options.verbose
	server := http.Server{
		Addr:         fmt.Sprintf(":%d", options.http),
		Handler:      s,
		ReadTimeout:  ReadTimeout,
		WriteTimeout: WriteTimeout,
	}
	if !options.ssl {
		return server.ListenAndServe()
	}
	if len(options.certificate) == 0 {
		return fmt.Errorf("Missing SSL certificate")
	}
	if len(options.key) == 0 {
		return fmt.Errorf("Missing SSL key")
	}
	return server.ListenAndServeTLS(options.certificate, options.key)
}

func (it *HttpProxy) FindProxy(url string) *ProxyContext {
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

func (it *HttpProxy) ServeHTTP(w http.ResponseWriter, r *http.Request) {

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
	if len(r.URL.Path) < len(ctx.Prefix) {
		http.Redirect(w, r, ctx.Prefix, http.StatusMovedPermanently)
		return
	}

	ip := it.SetRemoteAddress(r)
	prev := r.URL.Path
	r.URL.Path = r.URL.Path[len(ctx.Prefix):]
	if it.verbose > 0 {
		log.Println(prev, "-> http://"+ctx.Host+":"+ctx.Port+"/"+r.URL.Path, "from", ip)
	}
	r.Header.Set("Remote-Address", ip)
	ctx.proxy.ServeHTTP(w, r)
}

func (it *HttpProxy) SetRemoteAddress(r *http.Request) string {
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

func (it *HttpProxy) Register(q url.Values) error {
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
		Prefix: prefix,
		Host:   host,
		Port:   port,
	}
	log.Println("added proxy", prefix, "to", url)
	return nil
}

func (it *HttpProxy) Unregister(q url.Values) {
	prefix := GetPrefix(q.Get("prefix"))
	it.access.Lock()
	delete(it.targets, prefix)
	it.access.Unlock()
	log.Println("removed proxy", prefix)
}

func (it *HttpProxy) List(w http.ResponseWriter) {
	it.access.RLock()
	defer it.access.RUnlock()
	dst := []*ProxyContext{}
	for _, v := range it.targets {
		dst = append(dst, v)
	}
	bytes, err := json.MarshalIndent(dst, "", "\t")
	if err != nil {
		return
	}
	fmt.Fprintln(w, string(bytes))
}
