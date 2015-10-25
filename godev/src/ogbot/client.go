package main

import (
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"math/rand"
	"net/http"
	"net/http/cookiejar"
	"net/http/httputil"
	"net/url"
	"os"
	"runtime"
	"strconv"
	"strings"
	"time"
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
	//204 -> 10 means 10 ships of type 204
	DockedFleet map[string]int
}

type GameData struct {
	planets map[string]Planet
}

func (g *GameData) print(logger Logger) {
	for k, v := range g.planets {
		numShips := 0
		for _, num := range v.DockedFleet {
			numShips += num
		}
		logger.Printf("(%s) Planet [%s]: %dM, %dC, %dD. %d ships docked", k, v.Coordinates, v.Resources.Metal,
			v.Resources.Crystal, v.Resources.Deuterium, numShips)
	}
}

type MetaData struct {
	login string
	pass  string
	uni   string
	lang  string
}

const (
	AttackMovement    = 1
	TransportMovement = 3
	SpyMovement       = 6
)

type FleetMovement struct {
	id      string
	from    string
	to      string
	move    int // MovementType
	arrival string
	hostile bool
}

func (f *FleetMovement) print(logger Logger) {
	var hostile string
	if f.hostile {
		hostile = "/!\\"
	}
	logger.Printf("(%s) at %s [%s] -> [%s] (%s) %s", f.id, f.arrival,
		f.from, f.to, strconv.Itoa(f.move), hostile)
}

type OGBot struct {
	meta         MetaData
	data         GameData
	current      Page
	cookieHeader []string
	client       *http.Client
	logger       Logger
	dump         bool
	fleets       []*FleetMovement
}

func (b *OGBot) printFleets() {
	b.logger.Printf("%v fleet(s) movement(s)", len(b.fleets))
	for _, v := range b.fleets {
		v.print(b.logger)
	}
}

func countWords(word, content string) int {
	count := strings.Count(content, word)
	return count
}

func isLogginPage(page string) bool {
	if strings.Contains(page, "loginForm") {
		return true
	}
	return false
}

func logMark(mark string, logger Logger) {
	logger.Printf("// %s\n", mark)
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

func makeHttpClient(logger Logger, dump bool) *http.Client {
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
		if countWords("PHPSESSID", req.URL.String()) > 0 {
			logMark("Stop redirection...", logger)
			return fmt.Errorf("stop redirect")
		}
		if dump {
			dumpRequest(req, logger)
		}
		return nil
	}
	return client
}

func makeLoginRequest(login, pass, universe, lang string, logger Logger, dump bool) *http.Request {
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
	if dump {
		dumpRequest(req, logger)
	}
	return req
}

func (b *OGBot) makePageRequest(page, args string) *http.Request {
	req, _ := http.NewRequest("GET", "http://"+b.meta.uni+"-"+b.meta.lang+".ogame.gameforge.com/game/index.php?page="+page+args, nil)
	// set up basic header
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")
	req.Header["Cookie"] = b.cookieHeader
	return req
}

func (b *OGBot) goToPage(label, args string) {
	// make the overview request
	req := b.makePageRequest(label, args)
	logMark(label+" page request...", b.logger)
	resp, _ := b.client.Do(req)
	if b.dump {
		dumpResponse(resp, b.logger)
	}

	defer resp.Body.Close()
	contents, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		b.logger.Printf("%s\n", err.Error())
		return
	}
	b.current.pageType = label
	b.current.content = string(contents)
}

func getAttributeValue(att, content string) string {
	split := strings.Split(content, att+"=\"")
	if len(split) < 2 {
		return ""
	}
	final := strings.Split(split[1], "\"")
	return final[0]
}

func getFleetDataValue(att, content, sep1, sep2 string) string {
	split := strings.Split(content, att)
	second := strings.Split(split[1], sep1)
	final := strings.Split(second[1], sep2)
	return final[0]
}

func getFleetMovementInfo(content string) *FleetMovement {
	move := &FleetMovement{}
	move.hostile = strings.Contains(content, "countDown hostile")

	eventRowId := getAttributeValue("id", content)
	split := strings.Split(eventRowId, "-")
	move.id = split[1]
	mission, err := strconv.Atoi(getAttributeValue("data-mission-type", content))
	if err != nil {
		return nil
	}
	move.move = mission
	move.from = getFleetDataValue("coordsOrigin", content, "[", "]")
	move.to = getFleetDataValue("destCoords", content, "[", "]")
	move.arrival = getFleetDataValue("arrivalTime", content, ">", " ")
	return move
}

func (b *OGBot) ChecksFleetMovements() {
	b.sleep(20)
	b.goToPage("eventList", "&ajax=1")
	split := strings.Split(b.current.content, "eventFleet")
	if len(split) == 0 {
		b.fleets = nil
		return
	}
	b.fleets = make([]*FleetMovement, 0)
	for _, v := range split[1:] {
		b.fleets = append(b.fleets, getFleetMovementInfo(v))
	}
}

// get planet data: planet info, ressources and docked ships
func (b *OGBot) goToPlanet(id string) {
	b.goToPage("fleet1", "&cp="+id)
}

func (b *OGBot) login() {
	// make the login request
	req := makeLoginRequest(b.meta.login, b.meta.pass, b.meta.uni, b.meta.lang, b.logger, b.dump)
	logMark("Login request...", b.logger)
	resp, _ := b.client.Do(req)
	if b.dump {
		dumpResponse(resp, b.logger)
	}
	b.cookieHeader = resp.Header["Set-Cookie"]

	b.goToPage("overview", "")
	if isLogginPage(b.current.content) {
		b.logger.Printf("not logged in\n")
	}
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

func getDockedFleet(content string) map[string]int {
	dockedFleet := make(map[string]int)
	split := strings.Split(content, "#shipsChosen")
	for _, v := range split[1:] {
		shipData := strings.Split(v, ",")
		shipId := removeNoise(shipData[1])
		splitnumber := strings.Split(shipData[2], ")")
		number, err := strconv.Atoi(splitnumber[0])
		if err != nil {
			return nil
		}
		dockedFleet[shipId] = number
	}
	return dockedFleet
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
		DockedFleet: getDockedFleet(b.current.content),
	}
}

func (b *OGBot) UpdatePlanetsData() {
	planets := listAvailablePlanetIds(b.current.content)
	for _, v := range planets {
		b.sleep(20)
		b.goToPlanet(v)
		b.UpdatePlanetData()
	}
}

func (b *OGBot) ChecksReconnect() {
	b.logger.Printf("checks if still logged in...")
	b.goToPage("overview", "")
	if isLogginPage(b.current.content) {
		b.logger.Printf("no more logged in... trying to reconnect...")
		b.login()
	}
}

func (b *OGBot) sleep(amount int) {
	random := rand.Intn(amount)
	b.logger.Printf("Random sleep: %+v seconds", random)
	time.Sleep(time.Second * time.Duration(random))
}

func (b *OGBot) Run() {
	b.login()
	b.UpdatePlanetsData()

	// checks frequently we are logged in
	ticker := time.NewTicker(1789 * time.Second)
	defer ticker.Stop()
	for _ = range ticker.C {
		b.sleep(1000)
		b.ChecksReconnect()
		b.UpdatePlanetsData()
		b.ChecksFleetMovements()
		b.data.print(b.logger)
		b.printFleets()
	}
}

func makeOGBot(login, pass, uni, lang string, logger Logger, dump bool) *OGBot {
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
		client: makeHttpClient(logger, dump),
		logger: logger,
		dump:   dump,
	}
}

func main() {
	login := flag.String("login", "", "login")
	pass := flag.String("pass", "", "password")
	universe := flag.String("uni", "", "universe. Ex: s131, s132...")
	lang := flag.String("lang", "", "language. Ex: en, fr...")
	logFile := flag.String("log", "", "optional log filename")
	dump := flag.Bool("dump", false, "dump http requests and responses")
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

	b := makeOGBot(*login, *pass, *universe, *lang, logger, *dump)
	b.Run()
}
