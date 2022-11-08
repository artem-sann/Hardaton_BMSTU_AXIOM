package model

type Params struct {
	Name     string
	Surname  string
	FuelType string
	Volume   string
	Checkbox string
}

type Order struct {
	OrderID    int
	TankerID   int
	OrderTime  string
	PaidSucces bool
	Params     Params
}

type User struct {
	id      int
	name    string
	surname string
}

func GetAllUsers() (users []User, err error) {
	users = []User{
		{1, "Джон", "До"},
		{2, "Говард", "Рорк"},
		{3, "Джек", "Доусон"},
		{4, "Лизель", "Мемингер"},
		{5, "Джейн", "Эйр"},
		{6, "Мартин", "Иден"},
		{7, "Джон", "Голт"},
		{8, "Сэмвелл", "Тарли"},
		{9, "Гермиона", "Грейнджер"},
	}
	return
}
