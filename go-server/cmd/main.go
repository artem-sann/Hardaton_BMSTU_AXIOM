package main

import (
	"log"
	"net/http"

	"github.com/artem-sann/Hardaton_BMSTU_AXIOM/go-server/pkg/controller"
	"github.com/julienschmidt/httprouter"
)

func main() {
	//создаем и запускаем в работу роутер для обслуживания запросов
	r := httprouter.New()
	routes(r)
	//прикрепляемся к хосту и свободному порту для приема и обслуживания входящих запросов
	//вторым параметром передается роутер, который будет работать с запросами
	err := http.ListenAndServe(controller.URL, r)
	if err != nil {
		log.Fatal(err)
	}
}

func routes(r *httprouter.Router) {
	//путь к папке со внешними файлами: html, js, css, изображения и т.д.
	r.ServeFiles("/public/*filepath", http.Dir("public"))
	//что следует выполнять при входящих запросах указанного типа и по указанному адресу
	r.GET("/o", controller.OrderPage)
	//r.POST("/o", controller.OrderInputPage)
	r.POST("/done", controller.OrderDonePage)
	r.GET("/type", controller.OrderTypePage)
	r.GET("/sub", controller.OrderSubPage)
	r.GET("/g", controller.Get)
}
