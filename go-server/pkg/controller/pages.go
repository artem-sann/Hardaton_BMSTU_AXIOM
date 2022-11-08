package controller

import (
	"container/ring"
	"encoding/json"
	"html/template"
	"net/http"
	"path/filepath"

	"github.com/julienschmidt/httprouter"
)

const URL = "192.168.1.4:4444"

var Buf *ring.Ring

func init() {
	Buf = ring.New(2)
	Buf.Value = Params{}
	Buf.Next()
	Buf.Value = Params{}
}

func OrderPage(rw http.ResponseWriter, r *http.Request, p httprouter.Params) {
	path := filepath.Join("public", "html", "index.html")
	tmpl, err := template.ParseFiles(path)
	if err != nil {
		http.Error(rw, err.Error(), 400)
		return
	}
	err = tmpl.Execute(rw, nil) //выводим шаблон клиенту в браузер
	if err != nil {
		http.Error(rw, err.Error(), 400)
		return
	}
}

func OrderTypePage(rw http.ResponseWriter, r *http.Request, p httprouter.Params) {

	path := filepath.Join("public", "html", "type.html")
	tmpl, err := template.ParseFiles(path)
	if err != nil {
		http.Error(rw, err.Error(), 400)
		return
	}

	err = tmpl.Execute(rw, nil) //выводим шаблон клиенту в браузер
	if err != nil {
		http.Error(rw, err.Error(), 400)
		return
	}

}

func OrderSubPage(rw http.ResponseWriter, r *http.Request, p httprouter.Params) {
	FuilType := r.URL.Query().Get("fuil")
	par := Buf.Value.(Params)
	if !(par.Email == "" || par.Name == "" || par.Volume == "") {
		Am, err := CalculateAmount(FuilType, par.Volume)
		if err != nil {
			http.Error(rw, err.Error(), 400)
			return
		}
		par.Flag = true
		par.FuilType = FuilType
		Buf.Value = par
		http.Redirect(rw, r, "https://oplata.qiwi.com/create?publicKey=48e7qUxn9T7RyYE1MVZswX1FRSbE6iyCj2gCRwwF3Dnh5XrasNTx3BGPiMsyXQFNKQhvukniQG8RTVhYm3iPri1hpi3qPm67jCnJPQHJLFnY5KFV8QWqY97JHuHCNaBhpn8FWF2SdrPckjJxwNgb8RMrdowYkeo6U9CwZ5GY3BjUnA5HcDCqFGwSSEXip&amount="+Am, http.StatusSeeOther)
	} else {
		path := filepath.Join("public", "html", "type.html")
		tmpl, err := template.ParseFiles(path)
		if err != nil {
			http.Error(rw, err.Error(), 400)
			return
		}

		err = tmpl.Execute(rw, nil) //выводим шаблон клиенту в браузер
		if err != nil {
			http.Error(rw, err.Error(), 400)
			return
		}
	}

}

func Get(rw http.ResponseWriter, r *http.Request, p httprouter.Params) {
	par := Buf.Value.(Params)

	if par.Flag {
		err := json.NewEncoder(rw).Encode(Buf.Value)
		if err != nil {
			http.Error(rw, err.Error(), 400)
			return
		}
		Buf.Next()
		Buf.Value = Params{}
	} else {
		err := json.NewEncoder(rw).Encode(nil)
		if err != nil {
			http.Error(rw, err.Error(), 400)
			return
		}
	}
}

//48e7qUxn9T7RyYE1MVZswX1FRSbE6iyCj2gCRwwF3Dnh5XrasNTx3BGPiMsyXQFNKQhvukniQG8RTVhYm3iPri1hpi3qPm67jCnJPQHJLFnY5KFV8QWqY97JHuHCNaBhpn8FWF2S7z91f8JeUAyya4cTZz4hGJUVZnhGBCyjdHH7BjodeYP5Mmga5ypcB

//eyJ2ZXJzaW9uIjoiUDJQIiwiZGF0YSI6eyJwYXlpbl9tZXJjaGFudF9zaXRlX3VpZCI6ImZ3bjZjZC0wMCIsInVzZXJfaWQiOiI3OTE2NDY2MDQ2OSIsInNlY3JldCI6IjI3ZjYyNTE5N2EwYTVmZDQxMzg0ZmM4NmE4ZjA4OTY0Yjg5MDlkNTA2NTYyMDZiZGY4NjlkYWYyZjVjYTkxNDMifX0=
