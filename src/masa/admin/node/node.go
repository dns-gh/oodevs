package main

import (
	"compress/gzip"
	"encoding/json"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net"
	"net/http"
	"net/url"
	"os"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
	"text/template"
	"time"
)

func main() {
	log.Println("Sword Node - copyright Masa Group 2013")

	runtime.GOMAXPROCS(runtime.NumCPU())

	debug := flag.Bool("debug", false, "debug mode")
	host := flag.Int("host", 0, "")
	name := flag.String("name", "", "node name")
	node := flag.String("type", "cluster", "node type")
	port := flag.Int("port", 0, "listening port")
	reset := flag.Bool("reset", false, "allow session reset")
	tcp := flag.Int("tcp", 0, "Tcp host port")
	uuid := flag.String("uuid", "", "node uuid")
	www := flag.String("www", "www", "www directory")
	flag.Parse()

	log.Println("debug", *debug)
	log.Println("host", *host)
	log.Println("name", *name)
	log.Println("port", *port)
	log.Println("reset", *reset)
	log.Println("tcp", *tcp)
	log.Println("type", *node)
	log.Println("uuid", *uuid)
	log.Println("www", *www)

	handler := NewHandler(*debug, *reset, *host, *tcp, *name, *node, *uuid, *www)
	server := &http.Server{
		Addr:           fmt.Sprintf(":%d", *port),
		Handler:        handler,
		ReadTimeout:    5 * time.Minute,
		WriteTimeout:   5 * time.Minute,
		MaxHeaderBytes: http.DefaultMaxHeaderBytes,
	}

	err := server.ListenAndServe()
	if err != nil {
		log.Fatal(err)
	}
}

type Handler struct {
	http.Handler
	debug  bool
	reset  bool
	host   int
	tcp    int
	name   string
	node   string
	uuid   string
	www    string
	root   *template.Template
	client http.Client
}

func loadTemplates(pattern string) (*template.Template, error) {
	helpers := GetHelpers()
	t := template.New("index").Delims("[[", "]]").Funcs(helpers)
	t, err := t.ParseGlob(pattern + "/*.ttml")
	return t, err
}

func NewHandler(debug, reset bool, host, tcp int, name, node, uuid, www string) *Handler {
	it := &Handler{
		debug: debug,
		reset: reset,
		host:  host,
		tcp:   tcp,
		name:  name,
		node:  node,
		uuid:  uuid,
		www:   www,
	}
	t, err := loadTemplates(www)
	if err != nil {
		log.Fatal(err)
	}
	it.root = t
	return it
}

func AcceptEncoding(r *http.Request, enc string) bool {
	tab, ok := r.Header[http.CanonicalHeaderKey("Accept-Encoding")]
	if !ok {
		return false
	}
	for _, it := range tab {
		if strings.Contains(it, enc) {
			return true
		}
	}
	return false
}

func IsTextFile(filename string) bool {
	ext := filepath.Ext(filename)
	// quick & fast method, a better way would be to detect mimetype
	return ext == ".js" || ext == ".css"
}

type ResponseWriter struct {
	http.ResponseWriter
}

type CompressFilter struct {
	io.WriteCloser
	ResponseWriter
}

func (it *Handler) ServeHTTP(w http.ResponseWriter, r *http.Request) {

	if r.Method != "GET" {
		ServeError(w, http.StatusBadRequest)
		return
	}

	if r.URL.Path == "/" {
		r.URL.Path += it.node + "_index"
	}

	filename := it.www + r.URL.Path
	fi, err := os.Stat(filename)
	if err != nil {
		it.ServeTemplate(w, r)
		return
	}
	if fi.IsDir() {
		http.NotFound(w, r)
		return
	}

	if IsTextFile(filename) && AcceptEncoding(r, "gzip") {
		w.Header().Set("Content-Encoding", "gzip")
		encoder := gzip.NewWriter(w)
		defer encoder.Close()
		w = CompressFilter{encoder, ResponseWriter{w}}
	}

	http.ServeFile(w, r, filename)
}

func ServeError(w http.ResponseWriter, code int) {
	http.Error(w, http.StatusText(code), code)
}

type User map[string]string

func (it *Handler) ServeTemplate(w http.ResponseWriter, r *http.Request) {

	params, err := url.ParseQuery(r.URL.RawQuery)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	user := it.IsAuthenticated(r, params)
	dst := r.URL.Path
	if user == nil {
		dst = "/login"
	} else if it.node == "cluster" && user["type"] != "administrator" {
		http.Redirect(w, r, "/"+user["node_ident"], http.StatusMovedPermanently)
		return
	} else if v, err := strconv.ParseBool(user["temporary"]); v && err == nil {
		dst = "/update_login"
	}

	ctx := it.root
	if it.debug {
		ctx, _ = loadTemplates(it.www)
	}
	ctx = ctx.Lookup(dst[1:] + ".ttml")
	if ctx == nil {
		http.NotFound(w, r)
		return
	}

	model := it.MakeModel(params, user)

	if AcceptEncoding(r, "gzip") {
		w.Header().Set("Content-Encoding", "gzip")
		w.Header().Set("Content-Type", "text/html")
		encoder := gzip.NewWriter(w)
		defer encoder.Close()
		err = ctx.Execute(encoder, model)
	} else {
		err = ctx.Execute(w, model)
	}
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func (it *Handler) MakeModel(params url.Values, user User) map[string]interface{} {
	url := make(map[string]interface{})
	for k, v := range params {
		url[k] = v[0]
	}
	model := make(map[string]interface{})
	model["url"] = url
	model["tcp"] = it.tcp
	model["uuid"] = it.uuid
	model["name"] = it.name
	model["type"] = it.node
	model["debug"] = it.debug
	model["user"] = user
	model["reset"] = it.reset
	return model
}

func GetSid(r *http.Request, params url.Values) string {
	ck, err := r.Cookie("sid")
	if err == nil {
		return ck.Value
	}
	v, ok := params["sid"]
	if ok {
		return v[0]
	}
	return ""
}

func (it *Handler) IsAuthenticated(r *http.Request, params url.Values) User {
	sid := GetSid(r, params)
	if len(sid) == 0 {
		return nil
	}
	req, err := http.NewRequest("GET", fmt.Sprintf("http://localhost:%d/is_authenticated?sid=%s", it.host, sid), nil)
	if err != nil {
		return nil
	}
	remote := r.Header.Get("Remote-Address")
	if len(remote) > 0 {
		req.Header.Add("Remote-Address", remote)
	} else if len(r.RemoteAddr) > 0 {
		ip, _, err := net.SplitHostPort(r.RemoteAddr)
		if err != nil {
			req.Header.Add("Remote-Address", ip)
		}
	}
	rpy, err := it.client.Do(req)
	if err != nil || rpy.StatusCode != http.StatusOK {
		return nil
	}
	defer rpy.Body.Close()
	body, err := ioutil.ReadAll(rpy.Body)
	if err != nil {
		return nil
	}
	var user User
	err = json.Unmarshal(body, &user)
	if err != nil {
		return nil
	}
	return user
}
