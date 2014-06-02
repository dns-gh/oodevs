// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package web

import (
	"archive/zip"
	"bytes"
	"code.google.com/p/go.net/websocket"
	"compress/gzip"
	"errors"
	"fmt"
	"github.com/emicklei/go-restful"
	"io"
	"io/ioutil"
	"masa/timeline/i18n"
	"masa/timeline/sdk"
	"masa/timeline/server"
	"masa/timeline/util"
	"net/http"
	"net/url"
	"os"
	"path/filepath"
	"strings"
	"text/template"
	"time"
)

const (
	ReadTimeout  = 10 * time.Second
	WriteTimeout = 10 * time.Second
)

var (
	ErrTryTemplate = errors.New("try template")
	ErrUnknown     = errors.New("unknown")
)

type VirtualFile struct {
	data []byte
	mod  time.Time
}

type Server struct {
	debug        bool
	root         string
	controller   server.SdkController
	virtuals     map[string]VirtualFile
	templates    *template.Template
	service      restful.WebService
	translations *i18n.Translations
	socket       http.Handler
}

type RestHandler func(*restful.Request) (interface{}, error)

func wrap(handler RestHandler) restful.RouteFunction {
	return func(req *restful.Request, res *restful.Response) {
		body, err := handler(req)
		writeEntity(res, body, err)
	}
}

func (s *Server) AttachRoutes() {
	ws := restful.WebService{}
	ws.Path("/api").
		Consumes(restful.MIME_JSON).
		Produces(restful.MIME_JSON)
	ws.
		Route(ws.GET("/sessions").
		To(wrap(s.readSessions)).
		Doc("get all sessions"))
	ws.
		Route(ws.GET("/sessions/{uuid}").
		To(wrap(s.readSession)).
		Doc("get a session").
		Param(ws.PathParameter("uuid", "session identifier")))
	ws.
		Route(ws.PUT("/sessions/{uuid}").
		To(wrap(s.updateSession)).
		Doc("update a session").
		Param(ws.PathParameter("uuid", "session identifier")))
	ws.
		Route(ws.GET("/sessions/{uuid}/export").
		To(wrap(s.exportSession)).
		Doc("export a session").
		Param(ws.QueryParameter("sword_profile", "filter on selected sword profile")).
		Param(ws.PathParameter("uuid", "session identifier")))
	ws.
		Route(ws.POST("/sessions/{uuid}/import").
		To(wrap(s.importSession)).
		Doc("import a session").
		Param(ws.PathParameter("uuid", "session identifier")))
	ws.
		Route(ws.GET("/sessions/{uuid}/services").
		To(wrap(s.readServices)).
		Doc("get all services attached to this session").
		Param(ws.PathParameter("uuid", "session identifier")))
	ws.
		Route(ws.POST("/sessions/{uuid}/events").
		To(wrap(s.createEvent)).
		Doc("create an event").
		Param(ws.PathParameter("uuid", "session identifier")))
	ws.
		Route(ws.POST("/sessions/{uuid}/bulk_events").
		To(wrap(s.createEvents)).
		Doc("create multiple events").
		Param(ws.PathParameter("uuid", "session identifier")))
	ws.
		Route(ws.GET("/sessions/{uuid}/events").
		To(wrap(s.readEvents)).
		Doc("get all events attached to this session").
		Param(ws.QueryParameter("sword_profile", "filter on selected sword profile")).
		Param(ws.PathParameter("uuid", "session identifier")))
	ws.
		Route(ws.PUT("/sessions/{session}/events/{uuid}").
		To(wrap(s.updateEvent)).
		Doc("update an event").
		Param(ws.PathParameter("session", "session identifier")).
		Param(ws.PathParameter("uuid", "event identifier")))
	ws.
		Route(ws.DELETE("/sessions/{session}/events/{uuid}").
		To(wrap(s.deleteEvent)).
		Doc("delete an event").
		Param(ws.PathParameter("session", "session identifier")).
		Param(ws.PathParameter("uuid", "event identifier")))
	s.service = ws
}

func loadVirtuals(filename string) (map[string]VirtualFile, error) {
	virtuals := map[string]VirtualFile{}
	_, err := os.Stat(filename)
	if os.IsNotExist(err) {
		return virtuals, nil
	}
	r, err := zip.OpenReader(filename)
	if err != nil {
		return nil, err
	}
	defer r.Close()
	for _, file := range r.File {
		if file.FileInfo().IsDir() {
			continue
		}
		rc, err := file.Open()
		if err != nil {
			return nil, err
		}
		defer rc.Close()
		data, err := ioutil.ReadAll(rc)
		if err != nil {
			return nil, err
		}
		virtuals["/"+file.Name] = VirtualFile{data, file.ModTime()}
	}
	return virtuals, nil
}

func loadTemplates(root string, virtuals map[string]VirtualFile, translations *i18n.Translations) (*template.Template, error) {
	helpers := GetHelpers()
	helpers["i18n"] = func(source string) string {
		return source
	}
	t := template.New("index").Delims("[[", "]]").Funcs(helpers)
	if len(root) > 0 {
		return t.ParseGlob(root + "/*.ttml")
	}
	for name, fs := range virtuals {
		if !strings.HasSuffix(name, ".ttml") {
			continue
		}
		text := string(fs.data)
		name = filepath.Base(name)
		current := t
		if name != t.Name() {
			current = t.New(name)
		}
		_, err := current.Parse(text)
		if err != nil {
			return nil, err
		}
	}
	return t, nil
}

func loadTranslations(www string, virtuals map[string]VirtualFile) (*i18n.Translations, error) {
	if len(www) > 0 {
		return i18n.ReadTranslationFiles(filepath.Join(www, "locale"))
	}
	m := make(i18n.Translations)
	for name, fs := range virtuals {
		if !strings.HasSuffix(name, ".ts") {
			continue
		}
		translationFile, err := i18n.ReadTranslationData(fs.data)
		if err != nil {
			return nil, errors.New(fmt.Sprintf("failed to parse ts file: %v", err))
		}
		_, ok := m[translationFile.Lang]
		if ok {
			return nil, errors.New(fmt.Sprintf("translations already loaded for language: %s", translationFile.Lang))
		}
		m[translationFile.Lang] = *translationFile
	}
	return &m, nil
}

func NewServer(log util.Logger, debug bool, port int, pak, www string, controller server.SdkController) (*http.Server, error) {
	virtuals, err := loadVirtuals(pak)
	if err != nil {
		return nil, err
	}
	translations, err := loadTranslations(www, virtuals)
	if err != nil {
		return nil, err
	}
	templates, err := loadTemplates(www, virtuals, translations)
	if err != nil {
		return nil, err
	}
	handler := &Server{
		debug:        debug,
		root:         www,
		controller:   controller,
		virtuals:     virtuals,
		templates:    templates,
		translations: translations,
	}
	handler.AttachRoutes()
	handler.socket = websocket.Handler(func(ws *websocket.Conn) {
		handler.socketHandler(log, ws)
	})
	return &http.Server{
		Addr:           fmt.Sprintf(":%d", port),
		Handler:        handler,
		ReadTimeout:    ReadTimeout,
		WriteTimeout:   WriteTimeout,
		MaxHeaderBytes: http.DefaultMaxHeaderBytes,
	}, nil
}

func acceptEncoding(r *http.Request, enc string) bool {
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

func isTextFile(filename string) bool {
	ext := filepath.Ext(filename)
	// quick & fast method, a better way would be to detect mimetype
	return ext == ".js" || ext == ".css"
}

type NopCloser struct {
	io.Writer
}

func (NopCloser) Close() error {
	return nil
}

type ResponseWriter struct {
	http.ResponseWriter
}

type CompressFilter struct {
	io.WriteCloser
	ResponseWriter
}

func TryCompressWriter(w http.ResponseWriter, r *http.Request) CompressFilter {
	if !acceptEncoding(r, "gzip") {
		return CompressFilter{NopCloser{w}, ResponseWriter{w}}
	}
	w.Header().Set("Vary", "Accept-Encoding")
	w.Header().Set("Content-Encoding", "gzip")
	encoder := gzip.NewWriter(w)
	return CompressFilter{encoder, ResponseWriter{w}}
}

type ReadSeekerCloser struct {
	io.ReadSeeker
	io.Closer
}

func (s *Server) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	if strings.HasPrefix(r.URL.Path, "/api/") {
		out := TryCompressWriter(w, r)
		defer out.Close()
		restful.DispatchTo(s.service, out, r)
		return
	}
	if strings.HasPrefix(r.URL.Path, "/socket/") {
		s.socket.ServeHTTP(w, r)
		return
	}
	if r.URL.Path == "/" {
		r.URL.Path += "index"
	}
	mod, reader, err := s.serveFile(r.URL.Path)
	if err == ErrTryTemplate {
		s.serveTemplate(w, r)
		return
	}
	if err != nil {
		http.NotFound(w, r)
		return
	}
	defer reader.Close()
	if !s.debug {
		w.Header().Set("Expires", time.Now().AddDate(0, 1, 0).Format(time.RFC1123))
	}
	if isTextFile(r.URL.Path) {
		out := TryCompressWriter(w, r)
		defer out.Close()
		w = out
	}
	if filepath.Ext(r.URL.Path) == ".woff" {
		w.Header().Set("Content-Type", "application/x-font-woff")
	}
	http.ServeContent(w, r, r.URL.Path, mod, reader)
}

func (s *Server) serveFile(url string) (time.Time, *ReadSeekerCloser, error) {
	if len(s.root) > 0 {
		return s.serveLocalFile(url)
	}
	fs, ok := s.virtuals[url]
	if !ok {
		return time.Time{}, nil, ErrTryTemplate
	}
	reader := bytes.NewReader(fs.data)
	return fs.mod, &ReadSeekerCloser{reader, ioutil.NopCloser(reader)}, nil
}

func (s *Server) serveLocalFile(url string) (time.Time, *ReadSeekerCloser, error) {
	path := s.root + url
	fi, err := os.Stat(path)
	if err != nil {
		return time.Time{}, nil, ErrTryTemplate
	}
	if fi.IsDir() {
		return time.Time{}, nil, ErrUnknown
	}
	fs, err := os.Open(path)
	if err != nil {
		return time.Time{}, nil, err
	}
	return fi.ModTime(), &ReadSeekerCloser{fs, fs}, nil
}

func (s *Server) overwriteModelHelpers(ctx *template.Template, model *map[string]interface{}) {
	url := (*model)["url"].(map[string]interface{})
	lang := url["lang"].(string)
	ctx.Funcs(template.FuncMap{
		"i18n": func(source string) string {
			return s.translations.Tr(source, lang)
		},
	})
}

func (s *Server) serveTemplate(w http.ResponseWriter, r *http.Request) {
	params, err := url.ParseQuery(r.URL.RawQuery)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}
	dst := r.URL.Path
	ctx := s.templates
	if s.debug {
		ctx, err = loadTemplates(s.root, s.virtuals, s.translations)
		if err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
	}
	model := s.makeModel(params, s.root)
	s.overwriteModelHelpers(ctx, &model)
	ctx = ctx.Lookup(dst[1:] + ".ttml")
	if ctx == nil {
		http.NotFound(w, r)
		return
	}
	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	out := TryCompressWriter(w, r)
	defer out.Close()
	w = out
	err = ctx.Execute(w, model)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func (s *Server) makeModel(params url.Values, root string) map[string]interface{} {
	url := make(map[string]interface{})
	url["lang"] = "en"
	for k, v := range params {
		url[k] = v[0]
	}
	model := make(map[string]interface{})
	model["url"] = url
	model["debug"] = s.debug
	model["translations"] = s.translations
	return model
}

func writeEntity(res *restful.Response, value interface{}, err error) {
	if err != nil {
		writeError(res, err)
		return
	}
	res.WriteEntity(value)
}

func writeError(res *restful.Response, err error) {
	if code, _ := util.ConvertError(err); code != 0 {
		res.WriteError(int(code), err)
		return
	}
	res.WriteError(http.StatusInternalServerError, err)
}

func (s *Server) readSessions(req *restful.Request) (interface{}, error) {
	return s.controller.ListSessions(), nil
}

func (s *Server) readSession(req *restful.Request) (interface{}, error) {
	uuid := req.PathParameter("uuid")
	return s.controller.ReadSession(uuid)
}

func (s *Server) updateSession(req *restful.Request) (interface{}, error) {
	uuid := req.PathParameter("uuid")
	body := &sdk.Session{}
	err := req.ReadEntity(body)
	if err != nil {
		return nil, err
	}
	return s.controller.UpdateSession(uuid, body)
}

func exportEvents(events []*sdk.Event) ([]interface{}, error) {
	filter := server.NewSwordFilter()
	output := []interface{}{}
	for _, event := range events {
		data, err := filter.DecodeEvent(event)
		if err != nil {
			return nil, err
		}
		output = append(output, data)
	}
	return output, nil
}

func (s *Server) exportSession(req *restful.Request) (interface{}, error) {
	uuid := req.PathParameter("uuid")
	config, err := server.ParseEventFilterConfig(req.Request)
	if err != nil {
		return nil, err
	}
	events, err := s.controller.ReadEvents(uuid, config)
	if err != nil {
		return nil, err
	}
	return exportEvents(events)
}

func (s *Server) importEvents(session string, data []byte) ([]*sdk.Event, error) {
	filter := server.NewSwordFilter()
	events, err := filter.EncodeEvents(data)
	if err != nil {
		return nil, err
	}
	for _, it := range events {
		event, err := s.controller.UpdateEvent(session, it.GetUuid(), it)
		if err != nil {
			code, text := util.ConvertError(err)
			event.ErrorCode = &code
			event.ErrorText = &text
		}
		*it = *event
	}
	return events, nil
}

func (s *Server) importSession(req *restful.Request) (interface{}, error) {
	uuid := req.PathParameter("uuid")
	data, err := ioutil.ReadAll(req.Request.Body)
	if err != nil {
		return nil, err
	}
	return s.importEvents(uuid, data)
}

func (s *Server) readServices(req *restful.Request) (interface{}, error) {
	uuid := req.PathParameter("uuid")
	return s.controller.ReadServices(uuid)
}

func (s *Server) createEvent(req *restful.Request) (interface{}, error) {
	uuid := req.PathParameter("uuid")
	body := &sdk.Event{}
	err := req.ReadEntity(body)
	if err != nil {
		return nil, err
	}
	return s.controller.CreateEvent(uuid, body)
}

func (s *Server) createEvents(req *restful.Request) (interface{}, error) {
	uuid := req.PathParameter("uuid")
	body := []*sdk.Event{}
	err := req.ReadEntity(&body)
	if err != nil {
		return nil, err
	}
	for i, src := range body {
		dst, err := s.controller.CreateEvent(uuid, src)
		code, text := util.ConvertError(err)
		dst.ErrorCode = &code
		dst.ErrorText = &text
		body[i] = dst
	}
	return body, nil
}

func (s *Server) readEvents(req *restful.Request) (interface{}, error) {
	uuid := req.PathParameter("uuid")
	config, err := server.ParseEventFilterConfig(req.Request)
	if err != nil {
		return nil, err
	}
	return s.controller.ReadEvents(uuid, config)
}

func (s *Server) updateEvent(req *restful.Request) (interface{}, error) {
	session := req.PathParameter("session")
	uuid := req.PathParameter("uuid")
	body := &sdk.Event{}
	err := req.ReadEntity(body)
	if err != nil {
		return nil, err
	}
	return s.controller.UpdateEvent(session, uuid, body)
}

func (s *Server) deleteEvent(req *restful.Request) (interface{}, error) {
	session := req.PathParameter("session")
	uuid := req.PathParameter("uuid")
	return uuid, s.controller.DeleteEvent(session, uuid)
}
