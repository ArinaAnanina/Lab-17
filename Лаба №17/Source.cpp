#include <iostream>
#include <string>
using namespace std;

struct Date
{
	int Day, Month, Year;
	Date(int day, int month, int year)
	{
		this->Day = day;
		this->Month = month;
		this->Year = year;
	}
};

struct Human
{
	string FIO;
	Date* DateOfBirth;
	long long PassportNumber, PhoneNumber;

	Human()
	{
		string names[] = { "Иван", "Юрий", "Алексей", "Артём", "Дмитрий" };
		string surnames[] = { "Иванов" , "Ананин", "Тарасов" };
		string patronymic[] = { "Андреевич", "Вадимович", "Михаилович", "Кирилович", "Ильич" };

		this->FIO = surnames[rand() % 3] + " " + names[rand() % 5] + " " + patronymic[rand() % 5];
		this->DateOfBirth = new Date(1 + rand() % 28, 1 + rand() % 12, 1900 + rand() % 200);
		this->PassportNumber = 5700000000 + (rand() % 100) * 1000000 + rand() % 1000000;
		this->PhoneNumber = 89000000000 + (rand() % 100) * 10000000 + rand() % 1000000;
	}

	void Print()
	{
		cout << endl;
		cout << "ФИО: " << FIO << endl;
		cout << "Дата рождения: " << DateOfBirth->Day << "." << DateOfBirth->Month << "." << DateOfBirth->Year << endl;
		cout << "Номер паспорта: " << PassportNumber << endl;
		cout << "Номер телефона: " << PhoneNumber << endl;
	}
};

struct HashTableItem
{
	Human* data;
	HashTableItem* next;
	HashTableItem(Human* data, HashTableItem* next) {
		this->data = data;
		this->next = next;
	}
};

struct HashFunc
{
	double A = 0.00000001345666423;
	int operator()(long long value, int tableSize) const {
		float intprt;
		return trunc(tableSize * modf(value * this->A, &intprt));
	}
};

/*

const HashFuncByPhone& a = HashFuncByPhone();
a(123, 123);

*/

template <typename HashFunction>
struct HashTable {
	int tableSize;
	HashTableItem** hashTable;
	HashTable(int tableSize) {
		this->tableSize = tableSize;
		this->hashTable = new HashTableItem*[tableSize];
		for (int i = 0; i < tableSize; i++) {
			this->hashTable[i] = NULL;
		}
	}

	void Add(Human*& value, const HashFunction& func = HashFunction()) {
		int index = func(value->PhoneNumber, this->tableSize);
		HashTableItem* item = new HashTableItem(value, NULL);

		if (this->hashTable[index] == NULL) {
			this->hashTable[index] = item;
			return;
		}
		
		HashTableItem* place = this->hashTable[index];
		while (place->next != NULL) {
			place = place->next;
		}
		place->next = item;
	}

	void Delete(Human*& value, const HashFunction& func = HashFunction()) {

	}

	Human* Find(long long value, const HashFunction& func = HashFunction()) {
		int index = func(value, this->tableSize);

		if (this->hashTable[index] == NULL) {
			return NULL;
		}

		HashTableItem* item = this->hashTable[index];
		while (item != NULL) {
			if (item->data->PhoneNumber == value) {
				return item->data;
			}
			item = item->next;
		}
		return NULL;
	}

	void Print() {
		for (int i = 0; i < this->tableSize; i++) {
			cout << "INDEX: " << i << endl;

			if (this->hashTable[i] == NULL) {
				cout << "NULL";
			}
			else {
				HashTableItem* item = this->hashTable[i];
				while (item != NULL) {
					cout << item->data->FIO << ", " << item->data->PhoneNumber << endl;
					item = item->next;
				}
			}

			cout << endl << endl;
		}
	}
};

int main()
{
	system("color F0");
	setlocale(LC_ALL, "rus");
	HashTable<HashFunc>* table = new HashTable<HashFunc>(40);

	// Наполнение
	for (int i = 0; i < 100; i++) {
		Human* human = new Human();
		table->Add(human);
	}

	table->Print();

	// Коллизии
	int k = 0;
	for (int i = 0; i < table->tableSize; i++) {
		if (table->hashTable[i] != NULL && table->hashTable[i]->next != NULL) {
			k++;
		}
	}
	cout << "Кол-во коллизий: " << k;

	// Поиск
	long long phone;
	cout << endl << "Введите номер телефона по которому необходимо осуществить поиск" << endl;
	cin >> phone;

	Human* founded = table->Find(phone);

	if (founded != NULL) {
		founded->Print();
	}
	else {
		cout << "NOT FOUNDED" << endl;
	}

	return 0;
}