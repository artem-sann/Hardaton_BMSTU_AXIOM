FROM golang:alpine AS builder

RUN go version

COPY . /github.com/MrDjeb/tg-bot-kvartirant/
WORKDIR /github.com/MrDjeb/tg-bot-kvartirant/

RUN apk add build-base
#RUN go mod download go clean
RUN go clean
RUN GOOS=linux GOARCH=arm64 go build -o ./.bin cmd/main.go

FROM alpine:latest

WORKDIR /docker-tg-bot/

COPY --from=builder /github.com/MrDjeb/tg-bot-kvartirant/.bin .
COPY --from=builder /usr/local/go/lib/time/zoneinfo.zip /
COPY --from=builder /github.com/MrDjeb/tg-bot-kvartirant/configs configs/

ENV TZ="Europe/Moscow"
ENV ZONEINFO=/zoneinfo.zip
EXPOSE 80

ENTRYPOINT ["./.bin"]
