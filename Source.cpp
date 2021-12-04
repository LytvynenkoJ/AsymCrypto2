#include "Header.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	int choise = 0;

	cout << "Вы хотите сгенерировать случайный ключ для пользователя А или использовать свой?" << endl;
	cout << "0 - генерировать" << endl << "1 - использовать свой" << endl;
	cin >> choise;
	while (choise != 0 && choise != 1)
	{
		cout << "Выберете, пожалуйста, один из предложенных вариантов:        " << endl;
		cin >> choise;
	}
	string b;
	string n;
	int len = 10;
	if (choise == 0)
	{
		cout << "Введите желаемое количество байтов в случайных простых числах p, q:      ";
		cin >> len;
		GenerateKeyPair(len);
	}
	else
	{
		cout << "Введите, пожалуйста ключ n" << endl;
		cin >> n;
		len = n.length() / 2 - 2;
		GenerateKeyPair(len);
		nB = mpz_class(n, 16);
		cout << "Введите, пожалуйста ключ b" << endl;
		cin >> b;
		bB = mpz_class(b, 16);
		len = n.length()/2;
	}
	cout << endl << endl << "Открытые ключи:" << endl;
	cout << "nA = " << hex << nA.get_mpz_t() << endl << endl;
	cout << "bA = " << bA.get_mpz_t() << endl << endl;
	if (choise==1)
	{
		cout << endl << "Открытые ключи сервера:" << endl;
		cout << "nB = " << hex << nB.get_mpz_t() << endl << endl;
		cout << "bB = " << bB.get_mpz_t() << endl << endl;
	}


	cout << endl << endl << endl << "Введите длину желаемого сообщения (до " << dec << len - 10 << "-ти байтов): ";
	int l = 5;
	cin >> l;
	while (l<=0 || l>len-10)
	{
		cout << "Извините, но длина сообщения должна быть до " << len - 10 << "-ти байтов): " << endl;
		cout << "Ввёедите ещё раз длину сообщения:   ";
		cin >> l;
	}
	cout << endl << "Генерируем сообщение для шифрования/расшифрования" << endl << endl;
	l *= 2;
	int* number = new int[l];
	number = randomNum(l);
	string num = arrToStr(number, l);
	cout << "M  = " << num << endl;
	mpz_class M;
	//cout << "M_{format} = " << hex << M.get_mpz_t() << endl << endl;

	mpz_class C;
	if (choise==0)
	{
		num = format(num, len);
		M = mpz_class(num, 16);
		C = Encrypt(M, nA, bA);
		cout << "C = " << C.get_mpz_t() << endl;
		cout << "Parity = " << c1 << "         Jacobi = " << c2 << endl;
		M = Decrypt(C, nA, pA, qA, bA);
		//cout << hex << "M = " << M.get_mpz_t() << endl;
		M = unformat(M, len);
		cout << "После снятия форматирования сообщение: " << endl;
		cout << hex << "M = " << M.get_mpz_t() << endl << endl << endl;
	}
	else
	{
		M = mpz_class(num, 16);
		cout << "Получите на сайте шифротекст для сгенерированного сообщения" << endl;
		string c;
		cout << "Введите шифротекст" << endl;
		cin >> c;
		cout << "Введите символ четности" << endl;
		cin >> c1;
		cout << "Введите символ Якоби" << endl;
		cin >> c2;
		C = mpz_class(c, 16);
		mpz_class M2 = Decrypt(C, nA, pA, qA, bA);
		cout << "M_{format} = "  << hex << M2.get_mpz_t() << endl;
		M2 = unformat(M2, len-1);
		cout << "M = " << M2.get_mpz_t() << endl;
		if (mpz_cmp(M.get_mpz_t(), M2.get_mpz_t()) == 0)
		{
			cout << endl << "Вы правильно расшифровали текст!" << endl << endl << endl;
		}

		cout << "Зашифруем сообщение с помощью открытого ключа сервера" << endl;
		num = format(num, len);
		M = mpz_class(num, 16);
		cout << "M_{format} = " << M.get_mpz_t() << endl;
		C = Encrypt(M, nB, bB);
		cout << "C = " << C.get_mpz_t() << endl;
		cout << "Parity = " << c1 << "         Jacobi = " << c2 << endl;
		cout << "Пожалуйста расшифруйте на сайте и проверьте сообщение" << endl;
		cout << "Введите расшифрованное значение" << endl;
		cin >> num;
		M2 = mpz_class(num, 16);
		M = unformat(M, len);
		if (mpz_cmp(M.get_mpz_t(), M2.get_mpz_t()) == 0)
		{
			cout << endl << "Вы правильно зашифровали текст!" << endl << endl << endl;
		}
	}

	cout << endl << "Генерируем сообщение для подписи/проверки подписи" << endl << endl;
	number = randomNum(l);
	num = arrToStr(number, l);
	cout << "M  = " << num << endl;
	M = mpz_class(num, 16);
	if (choise==0)
	{
		C = Sign(num, nA, pA, qA, len);
		cout << "S = " << C.get_mpz_t() << endl;
		Verify(M, C, nA, len-1);
	}
	else
	{
		cout << "Пожалуйста, получите на сайте подпись к данному сообщению" << endl;
		string s;
		cin >> s;
		C = mpz_class(s, 16);
		Verify(M, C, nB, len);
		cout << endl << endl;
		cout << "Сейчас вы получите сообщение и подпись, проверьте, что подпись правильная" << endl;
		C = Sign(num, nA, pA, qA, len);
		cout << "M = " << M.get_mpz_t() << endl;
		cout << "S = " << C.get_mpz_t() << endl;
	}


	cout << endl << endl << endl << "Атака на протокол с нулевым разглашением" << endl << endl;
	ZeroKnowledge();

	return 0;
}
