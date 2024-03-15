#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

struct NumberInWords
{
    std::string one[10] = { "","один ","два ","три ","четыре " ,"пять " ,"шесть " ,"семь " ,"восемь " ,"девять " };
    std::string oneT[10] = { "", "одна ","две ","три ","четыре " ,"пять " ,"шесть " ,"семь " ,"восемь " ,"девять " };
    std::string ten[10] = { "","десять ", "двадцать ","тридцать ","сорок " ,"пятьдесят " ,"шестьдесят " ,"семьдесят " ,"восемьдесят " ,"девяносто " };
    std::string tens[10] = { "десять ","одиннадцать ","двенадцать ","тринадцать ","четырнадцать ","пятнадцать ","шестнадцать ","семнадцать ","восемнадцать ","девятнадцать " };
    std::string hundred[10] = { "", "сто ","двести ","триста ","четыреста " ,"пятьсот " ,"шестьсот " ,"семьсот " ,"восемьсот " ,"девятьсот " };
    std::string currency[3] = { "рублей", "рубля", "рубль" };
    std::string ranks[3][4] = { {"","тысяч ","тысячи ","тысяча "}, 
                                {"","миллионов ","миллиона ","миллион "}, 
                                {"","миллиардов ","миллиарда ","миллиард "} };
    std::string*** triadValues = new std::string * *[2];
    void arrFill()
    {
        for (int i = 0; i < 2; i++)
        {
            triadValues[i] = new std::string * [3];
            for (int j = 0; j < 3; j++)         //Заполняем массив для тысяч 1 массив, для остальных другой
                if (i == 0 && j == 2)           // поскольку у нас уже есть готовые массивы сверху, просто ссылаемся на них
                    triadValues[i][j] = oneT;   //Получится, что для тысяч массив с индексом[1][][], для остальных[0][][]
                else if (i == 1 && j == 2)      //Далее единицы [][2][], десятки [][1][], сотни [][0][]
                    triadValues[i][j] = one;    //В обращении к 3 пространству [][][n], мы указываем число и получаем тот элемент, который нам нужен
                else if (j == 1)
                    triadValues[i][j] = ten;
                else if (j == 0)
                    triadValues[i][j] = hundred;
        }
    }
    void arrInsertTen(int whichArr) { triadValues[whichArr][1] = ten; };
    void arrInsertTens(int whichArr) { triadValues[whichArr][1] = tens; };
    void arrDelete() { for (int i = 0; i < 2; i++) delete[] triadValues[i]; }
    std::string ending(int rank, int index, int num)
    {
        bool isN2One = num / 10 % 10 == 1;               
        if (rank == 0)
            return currency[index * !isN2One * !!num];
        return ranks[rank - 1][index * !isN2One + !!num ];
    }
};

NumberInWords NIW;

int IntegerLen(int num) 
{
    int k = 0;
    while (num > 0) {
        num /= 10;
        k++;
    }
    return k;
}

std::string valueSolver(int num, int rank)
{    
    std::string ans;
    int n1 = num / 100, n2 = num / 10 % 10, n3 = num % 10;
    
    switch (n3)
    {
    case 1: return NIW.ending(rank, 2, num);                                                              //Если проходит 1, то индекс будет 2
    case 2: case 3: case 4: return NIW.ending(rank, 1, num);                                          //Если проходит 2,3 или 4, то индекс будет 1
    default: return NIW.ending(rank, 0, num);
    }
    return ans;
}

std::string triadSolver(int num, int rank)
{
    std::string ans;
    int isThous = rank != 1;
    int n1 = num / 100, n2 = num / 10 % 10, n3 = num % 10; 
    ans = NIW.triadValues[isThous][0][n1];
    switch (n2)
    {
    case 1:
        NIW.arrInsertTens(isThous);
        ans += NIW.triadValues[isThous][1][n3];
        NIW.arrInsertTen(isThous);
        return ans;
    default:
        ans += NIW.triadValues[isThous][1][n2];
    }    
    ans += NIW.triadValues[isThous][2][n3];
    return ans;
}

bool programmExit()
{
    std::string num;
    std::cout << "\nЕсли хотите ввести ещё число, введите любое значение, если хотите выйти напишите \"Exit\"\n";
    std::cin >> num;
    if (num == "Exit") { NIW.arrDelete(); return false; }
    system("cls");
    return true;
}

void mainSolver()
{
    std::string ans = "";
    int num, tempString, tenThird = 1000, triad = 3;
    std::cout << "Введите число\n";
    std::cin >> num;
    ans = "";
    int numRank = (IntegerLen(num) + 2) / 3;  // numLen - длина строки, numRank - какой максимальный разряд (Миллиарды, Миллионы и т.д.)
    if (!num) ans = "ноль рублей";                    //Необходимый if, т.к. 0 - исключение
    for (int i = 0; i < numRank; ++i)                                       // 3 - отвечает за миллиарды 2 - миллионы, 1 - тысячи,  0 - рубли        
    {
        tempString = static_cast<int>(num / std::pow(10, triad * i)) % tenThird;    // Отделяем каждый раз 3 элемента и таким образом проходим всё число
        ans = triadSolver(tempString, i) + valueSolver(tempString, i) + ans;        
    }
    std::cout << ans << std::endl;
}

int main()
{
    setlocale(LC_ALL, "");
    NIW.arrFill();
    do
        mainSolver();
    while (programmExit());
}