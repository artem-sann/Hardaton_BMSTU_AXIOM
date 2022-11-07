package controller

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strconv"

	"github.com/julienschmidt/httprouter"
)

var Cost = map[int]int{
	80: 45,
	92: 47,
	95: 52,
	98: 63,
}

func CalculateAmount(FuelType, Volume string) (string, error) {
	Ful, err := strconv.Atoi(FuelType)
	if err != nil {
		return "", err
	}
	Vol, err := strconv.Atoi(Volume)
	if err != nil {
		return "", err
	}
	return strconv.Itoa(Cost[Ful] * Vol), nil
}

/*func OrderInputPage(rw http.ResponseWriter, r *http.Request, p httprouter.Params) {

	path := filepath.Join("public", "html", "order.html")
	tmpl, err := template.ParseFiles(path)
	if err != nil {
		http.Error(rw, err.Error(), 400)
		return
	}

	par := model.Params{Name: r.FormValue("name"), Surname: r.FormValue("surname"), FuelType: r.FormValue("fuel_type"), Volume: r.FormValue("volume"), Checkbox: r.FormValue("checkbox")}

	if par.Name == "" || par.Surname == "" || par.FuelType == "" || par.Volume == "" || par.Checkbox == "" {
		err = tmpl.Execute(rw, nil) //выводим шаблон клиенту в браузер
		if err != nil {
			http.Error(rw, err.Error(), 400)
			return
		}
	} else {
		Buf.Value = par
		Am, err := CalculateAmount(par.FuelType, par.Volume)
		if err != nil {
			http.Error(rw, err.Error(), 400)
			return
		}
		http.Redirect(rw, r, "https://oplata.qiwi.com/create?publicKey=48e7qUxn9T7RyYE1MVZswX1FRSbE6iyCj2gCRwwF3Dnh5XrasNTx3BGPiMsyXQFNKQhvukniQG8RTVhYm3iPri1hpi3qPm67jCnJPQHJLFnY5KFV8QWqY97JHuHCNaBhpn8FWF2SdrPckjJxwNgb8RMrdowYkeo6U9CwZ5GY3BjUnA5HcDCqFGwSSEXip&amount="+Am, http.StatusSeeOther)

	}
	/*fmt.Printf("Name: %s\nSurname: %s\nFuelType: %s\nVolume: %s\nCheckbox %s",
	r.FormValue("name"), r.FormValue("surname"), r.FormValue("fuel_type"), r.FormValue("volume"), r.FormValue("checkbox"))

}*/

type Params struct {
	Name     string `json:"Name"`
	Email    string `json:"Email"`
	Volume   string `json:"Volume"`
	FuilType string `json:"FuilType"`
	Flag     bool
}

func OrderDonePage(rw http.ResponseWriter, r *http.Request, p httprouter.Params) {

	var par Params

	// Try to decode the request body into the struct. If there is an error,
	// respond to the client with the error message and a 400 status code.
	err := json.NewDecoder(r.Body).Decode(&par)
	if err != nil {
		http.Error(rw, err.Error(), http.StatusBadRequest)
		return
	}

	par.Flag = false
	// Do something with the Person struct...
	fmt.Fprintf(rw, "Person: %+v", par)
	fmt.Println(par)

	Buf.Value = par
}

/*https://oplata.qiwi.com/create?
publicKey=5nAq6abtyCz4tcDj89e5w7Y5i524LAFmzrsN6bQTQ3ceEvMvCq55ToeErzhvK6rVkQLaCrYUQcYF5QkS8nCrjnPsLQgsLxqrpQgJ7hg2ZHmEHXFjaG8qjvgcep
&extras[cf1]=Order_123
&extras[cf3]=winnie@pooh.ru
&readonly_extras=cf1
&comment=some%20comment
&amount=100.00*/
//https://oplata.qiwi.com/create?publicKey=48e7qUxn9T7RyYE1MVZswX1FRSbE6iyCj2gCRwwF3Dnh5XrasNTx3BGPiMsyXQFNKQhvukniQG8RTVhYm3iPri1hpi3qPm67jCnJPQHJLFnY5KFV8QWqY97JHuHCNaBhpn8FWF2SdrPckjJxwNgb8RMrdowYkeo6U9CwZ5GY3BjUnA5HcDCqFGwSSEXip&amount=100&&extras[cf1]=Order_123
