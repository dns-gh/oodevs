package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/http/cookiejar"
	"net/http/httputil"
	"net/url"
	"strings"
)

type Page struct {
	pageType string
	content  string
}

type GameData struct {
	metal     int
	cristal   int
	deuterium int
}

type OGBot struct {
	data         GameData
	current      Page
	cookieHeader []string
}

func countWords(word, content string) int {
	count := strings.Count(content, word)
	fmt.Printf("'%s' appears %d time\n", word, count)
	return count
}

func (b *OGBot) isLoggedIn(page string) bool {
	if countWords("login", page) > 1 {
		return false
	}
	return true
}

func logMark(mark string) {
	fmt.Println("------------------------------------------------")
	fmt.Println(mark)
	fmt.Println("------------------------------------------------")
}

func dumpResponse(resp *http.Response) {
	dump, err := httputil.DumpResponse(resp, false)
	if err != nil {
		fmt.Printf("dumpResponse err: ", err.Error())
		return
	}
	fmt.Println("Response dump: \n", string(dump))
}

func dumpRequest(req *http.Request) {
	dump, err := httputil.DumpRequestOut(req, false)
	if err != nil {
		fmt.Printf("dumpRequest err: ", err.Error())
		return
	}
	fmt.Println("Request dump: \n", string(dump))
}

func makeHttpClient() *http.Client {
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
		logMark("Redirection...")
		fmt.Println("URL: ", req.URL.String())
		if countWords("PHPSESSID", req.URL.String()) > 0 {
			logMark("Stop redirection...")
			return fmt.Errorf("stop redirect")
		}
		dumpRequest(req)
		return nil
	}
	return client
}

func makeLoginRequest(login, pass, universe, lang string) *http.Request {
	// fill in login form
	data := &url.Values{}
	data.Add("kid", "")
	data.Add("uni", universe+"-"+lang+".ogame.gameforge.com")
	data.Add("login", login)
	data.Add("pass", pass)
	req, _ := http.NewRequest("POST", "http://en.ogame.gameforge.com/main/login",
		strings.NewReader(data.Encode()))
	// set up basic header
	req.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	req.Header.Add("Host", "en.ogame.gameforge.com")
	req.Header.Add("Connection", "keep-alive")
	req.Header.Add("Content-Length", "77")
	req.Header.Add("Pragma", "no-cache")
	req.Header.Add("Cache-Control", "no-cache")
	req.Proto = "HTTP/1.0"
	dumpRequest(req)
	return req
}

func (b *OGBot) makePageRequest(page string) *http.Request {
	req, _ := http.NewRequest("GET", "http://s131-en.ogame.gameforge.com/game/index.php?page="+page, nil)
	// set up basic header
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")
	req.Header["Cookie"] = b.cookieHeader
	return req
}

func (b *OGBot) login(login, pass, universe, lang string) error {
	client := makeHttpClient()
	// make the login request
	req := makeLoginRequest(login, pass, universe, lang)
	logMark("Login request...")
	resp, _ := client.Do(req)
	dumpResponse(resp)
	b.cookieHeader = resp.Header["Set-Cookie"]

	// make the overview request
	req = b.makePageRequest("overview")
	logMark("Overview panel request...")
	resp, _ = client.Do(req)
	dumpResponse(resp)

	defer resp.Body.Close()
	contents, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return err
	}
	b.current.pageType = "overview"
	b.current.content = string(contents)
	if !b.isLoggedIn(b.current.content) {
		return fmt.Errorf("Not logged in.\n")
	}
	return nil
}

func main() {
	login := flag.String("login", "", "login")
	pass := flag.String("pass", "", "password")
	universe := flag.String("uni", "", "universe. Ex: s131, s132...")
	lang := flag.String("lang", "", "language. Ex: en, fr...")
	flag.Parse()

	b := &OGBot{}
	err := b.login(*login, *pass, *universe, *lang)
	if err != nil {
		fmt.Printf("%s", err.Error())
	}
	countWords("overview", b.current.content)
	countWords("resources_metal", b.current.content)
	countWords("resourceSettings", b.current.content)
	countWords("solarscroll", b.current.content)
}
