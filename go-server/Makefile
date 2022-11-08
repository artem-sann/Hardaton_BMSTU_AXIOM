

build:
	go build -o ./.bin cmd/main.go

run: build
	./.bin

build-image:
	docker build -t mrdjeb/tg-bot-kvartirant:0.1 .

start-container:
	docker run --env-file .env -p 80:80 mrdjeb/tg-bot-kvartirant:0.1

push:
	go mod tidy | git add . | git commit | git push

build-image-r:
	docker build --platform linux/arm64/v8 -t mrdjeb/tg-bot-kvartirant:1.4 .