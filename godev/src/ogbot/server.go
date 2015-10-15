package main

import (
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"net/http/cookiejar"
	"net/http/httputil"
	"net/url"
	"os"
	"runtime"
	"strconv"
	"strings"
)

type Logger interface {
	Printf(format string, a ...interface{})
}

type Page struct {
	pageType string
	content  string
}

type Resources struct {
	Metal     int
	Crystal   int
	Deuterium int
}

type Planet struct {
	Name        string
	Coordinates string
	Resources   Resources
}

type GameData struct {
	planets map[string]Planet
}

func (g *GameData) print() {
	fmt.Printf("%+v\n", g)
}

type MetaData struct {
	login string
	pass  string
	uni   string
	lang  string
}

type OGBot struct {
	meta         MetaData
	data         GameData
	current      Page
	cookieHeader []string
	client       *http.Client
	logger       Logger
}

func countWords(word, content string) int {
	count := strings.Count(content, word)
	fmt.Printf("'%s' appears %d time\n", word, count)
	return count
}

func isLogginPage(page string) bool {
	if strings.Contains(page, "loginForm") {
		return true
	}
	return false
}

func logMark(mark string, logger Logger) {
	logger.Printf("------------------------------------------------\n")
	logger.Printf("%s\n", mark)
	logger.Printf("------------------------------------------------\n")
}

func dumpResponse(resp *http.Response, logger Logger) {
	dump, err := httputil.DumpResponse(resp, false)
	if err != nil {
		logger.Printf("dumpResponse err: %s\n", err.Error())
		return
	}
	logger.Printf("Response dump: %s\n", string(dump))
}

func dumpRequest(req *http.Request, logger Logger) {
	dump, err := httputil.DumpRequestOut(req, false)
	if err != nil {
		logger.Printf("dumpRequest err: %s", err.Error())
		return
	}
	logger.Printf("Request dump: %s\n", string(dump))
}

func makeHttpClient(logger Logger) *http.Client {
	// set basic cookie jar
	jar, _ := cookiejar.New(nil)
	var cookies []*http.Cookie
	firstCookie := &http.Cookie{
		Name:  "OG_lastServer",
		Value: "s131-en.ogame.gameforge.com",
	}
	cookies = append(cookies, firstCookie)
	cookieURL, _ := url.Parse("http://en.ogame.gameforge.com")
	jar.SetCookies(cookieURL, cookies)
	// set client to make the login request
	client := &http.Client{
		Jar: jar,
	}
	// set up redirection policy and break when encouters second redirection
	// in order to get the response header that contains cookie information
	// vital to make further requests
	client.CheckRedirect = func(req *http.Request, via []*http.Request) error {
		logMark("Redirection...", logger)
		logger.Printf("URL: %s\n", req.URL.String())
		if countWords("PHPSESSID", req.URL.String()) > 0 {
			logMark("Stop redirection...", logger)
			return fmt.Errorf("stop redirect")
		}
		dumpRequest(req, logger)
		return nil
	}
	return client
}

func makeLoginRequest(login, pass, universe, lang string, logger Logger) *http.Request {
	// fill in login form
	data := &url.Values{}
	data.Add("kid", "")
	data.Add("uni", universe+"-"+lang+".ogame.gameforge.com")
	data.Add("login", login)
	data.Add("pass", pass)
	req, _ := http.NewRequest("POST", "http://"+lang+".ogame.gameforge.com/main/login",
		strings.NewReader(data.Encode()))
	// set up basic header
	req.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	req.Header.Add("Host", "en.ogame.gameforge.com")
	req.Header.Add("Connection", "keep-alive")
	req.Header.Add("Content-Length", "77")
	req.Header.Add("Pragma", "no-cache")
	req.Header.Add("Cache-Control", "no-cache")
	req.Proto = "HTTP/1.0"
	dumpRequest(req, logger)
	return req
}

func (b *OGBot) makePageRequest(page string) *http.Request {
	req, _ := http.NewRequest("GET", "http://"+b.meta.uni+"-"+b.meta.lang+".ogame.gameforge.com/game/index.php?page="+page, nil)
	// set up basic header
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")
	req.Header["Cookie"] = b.cookieHeader
	return req
}

func (b *OGBot) login() error {
	// make the login request
	req := makeLoginRequest(b.meta.login, b.meta.pass, b.meta.uni, b.meta.lang, b.logger)
	logMark("Login request...", b.logger)
	resp, _ := b.client.Do(req)
	dumpResponse(resp, b.logger)
	b.cookieHeader = resp.Header["Set-Cookie"]

	// make the overview request
	req = b.makePageRequest("overview")
	logMark("Overview panel request...", b.logger)
	resp, _ = b.client.Do(req)
	dumpResponse(resp, b.logger)

	defer resp.Body.Close()
	contents, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return err
	}

	b.current.pageType = "overview"
	b.current.content = string(contents)
	if isLogginPage(b.current.content) {
		return fmt.Errorf("Not logged in...\n")
	}
	return nil
}

// remove spaces, dots and carriage returns
func removeNoise(value string) string {
	temp := strings.Replace(value, " ", "", -1)
	temp = strings.Replace(temp, ".", "", -1)
	return strings.Replace(temp, "\n", "", -1)
}

func getTagValue(tag, page string) (string, error) {
	split := strings.Split(page, tag)
	if len(split) != 2 {
		return "", fmt.Errorf("too many %s tags (%d)", tag, len(split)-1)
	}
	splitValue := strings.Split(split[1], ">")
	lastSplit := strings.Split(splitValue[1], "<")
	return removeNoise(lastSplit[0]), nil
}

func getResourceValue(resource, page string) int {
	value, err := getTagValue(resource, page)
	if err != nil {
		fmt.Println(err.Error())
		return -1
	}
	val, _ := strconv.Atoi(value)
	return val
}

func getMetaOGame(page, suffix string) string {
	split := strings.Split(page, "ogame-"+suffix)
	split = strings.Split(split[1], "\"")
	return split[2]
}

func listAvailablePlanetIds(page string) []string {
	split := strings.Split(page, "id=\"planet-")
	var list []string
	for _, value := range split[1:] {
		temp := strings.Split(value, "\"")
		list = append(list, temp[0])
	}
	return list
}

func getCurrentPlanet(page string) (string, string, string) {
	id := getMetaOGame(page, "planet-id")
	name := getMetaOGame(page, "planet-name")
	coord := getMetaOGame(page, "planet-coordinates")
	return id, name, coord
}

func (b *OGBot) UpdatePlanetData() {
	id, name, coord := getCurrentPlanet(b.current.content)
	b.data.planets[id] = Planet{
		Name:        name,
		Coordinates: coord,
		Resources: Resources{
			Metal:     getResourceValue("resources_metal", b.current.content),
			Crystal:   getResourceValue("resources_crystal", b.current.content),
			Deuterium: getResourceValue("resources_deuterium", b.current.content),
		},
	}
	fmt.Printf("%+v\n", b.data.planets)
}

func (b *OGBot) Run() {
	err := b.login()
	if err != nil {
		b.logger.Printf("%s", err.Error())
		return
	}
	b.UpdatePlanetData()
}

func makeOGBot(login, pass, uni, lang string, logger Logger) *OGBot {
	return &OGBot{
		meta: MetaData{
			login: login,
			pass:  pass,
			uni:   uni,
			lang:  lang,
		},
		data: GameData{
			planets: make(map[string]Planet),
		},
		client: makeHttpClient(logger),
		logger: logger,
	}
}

func main() {
	login := flag.String("login", "", "login")
	pass := flag.String("pass", "", "password")
	universe := flag.String("uni", "", "universe. Ex: s131, s132...")
	lang := flag.String("lang", "", "language. Ex: en, fr...")
	logFile := flag.String("log", "", "optional log filename")
	flag.Parse()
	runtime.GOMAXPROCS(runtime.NumCPU())

	logger := log.New(os.Stderr, "", log.LstdFlags)
	if len(*logFile) > 0 {
		file, err := os.Create(*logFile)
		if err != nil {
			log.Fatalln("unable to create file", *logFile)
		}
		defer file.Close()
		logger = log.New(io.MultiWriter(file, os.Stderr), "", log.LstdFlags)
	}
	logger.Println("login", *login)
	logger.Println("pass", "*n/a*")
	logger.Println("uni", *universe)
	logger.Println("lang", *lang)
	if len(*logFile) > 0 {
		logger.Println("log", *logFile)
	}

	b := makeOGBot(*login, *pass, *universe, *lang, logger)
	b.Run()

	countWords("overview", b.current.content)
	countWords("resources_metal", b.current.content)
	countWords("resourceSettings", b.current.content)
	countWords("solarscroll", b.current.content)
}
