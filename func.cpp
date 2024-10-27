#include "header.h"
mutex g_lock;

//Добавление в конец 1й связный колонок
void AddToEndColon(TablColonNode* ptr, string element){
    Node* new_node = new Node;
    new_node->tableColon = element;
    new_node->next = nullptr;
    if(ptr->fColon == nullptr){
        ptr->fColon = new_node;
    }else{

        Node* nextNode = ptr->fColon;
        while(nextNode-> next != nullptr){
            nextNode= nextNode->next;
        }
        nextNode->next = new_node;
    }
}

//Добавление в конец 1й связный таблиц
void AddToEndTabl(TablColonNode* ptr, string element){
    TablColonNode* new_node = new TablColonNode;
    new_node->table = element;
    new_node->next = nullptr;
    TablColonNode* nextNode = ptr;
    while(nextNode-> next != nullptr){
        nextNode= nextNode->next;
    }
        nextNode->next = new_node;
}


//Запись колонок и таблиц
TablColonNode* structure(string strTable, size_t i){
    TablColonNode* nameTab = new TablColonNode;
    nameTab->fColon = nullptr;


   //1 таблица
    while(strTable[i]!='"'){
            i++;
    }
    string meaning;
    for(size_t j=i+1; strTable[j]!='"';j++){
        meaning+=strTable[j];
        i=j;
    }

    nameTab->table = meaning;
    TablColonNode* nameTab2 = nameTab;
    int tablOrCol = 0;
    //далее
    for(;i<strTable.length(); i++){
        //ближайшие ковычки
        if(strTable[i]=='"'){
            if(strTable[i+1]!=','&& strTable[i+2]!=']'&& strTable[i+1]!=':'&& strTable[i+1]!=']'){
            string meaningNew ;
            //начало ли колонок
            if(strTable[i-1]=='['){
                tablOrCol=1;
            }
            for(size_t j=i+1; strTable[j]!='"';j++){
                meaningNew+=strTable[j];
                i=j;
            }

            //новая таблица
            if(strTable[i+2]==':'){
                tablOrCol=2;
            }

            //Добавление элемента
            if(tablOrCol==1){
                AddToEndColon(nameTab2, meaningNew);

            }else if(tablOrCol == 2 ){
                AddToEndTabl(nameTab, meaningNew);
                nameTab2 = nameTab->next;
                nameTab2->next = nullptr;
                tablOrCol=0;
            }
        }
     }

    }
    return nameTab;

}



//Запись с файла
void whithFile(configuration& cond){
    // окрываем файл для чтения
    ifstream inTable("schema.json");
    string strTable;

    if (inTable.is_open())
    {
        string temporary;
        while (getline(inTable, temporary))
        {
            strTable += temporary;

        }
    }

    // закрываем файл
    inTable.close();

    // идем по таблице
    for(size_t i=0;i< strTable.length(); i++){
     if(strTable[i]=='\"'){
      string word;
      for(size_t j=i+1; strTable[j]!='\"';j++){
        word+=strTable[j];
        i=j;
      }
      i+=2;

      //Записываем название

      if(word == "name"){
        while(strTable[i]!='"'){
            i++;
        }
        string meaning;
        for(size_t j=i+1; strTable[j]!='"';j++){
            meaning+=strTable[j];
            i=j;
        }
        i++;
        cond.name = meaning;

        //Записываем лимит

      }else if(word == "tuples_limit"){
        while(strTable[i]<'0'|| strTable[i]>'9'){
            i++;
        }
        string meaning;
         for(size_t j=i; strTable[j]!=',';j++){
            meaning+=strTable[j];
            i=j;
        }
        i++;
        cond.tuples_limit = stoi(meaning);

        //Таблицы
      }else if(word == "structure"){
            cond.Storage = structure(strTable, i);
        }
      }
    }
}

//Функция создания фалой и папок
void NewFile(configuration cond){
    TablColonNode* nameTab = cond.Storage;
    Node* nameCol = nameTab->fColon;
    //новая папка
    cout<<"Выберите: \n 1 - удалить содержимое файлов \n 2 - сохранить содержимое файлов\n";
    string DeleteOrNo="";
    cin>>DeleteOrNo;
     string str="";
    while(true){
        if(DeleteOrNo=="1"){
            str = "rm -r \"" + cond.name +  '\"';
            system(str.c_str());
            str = "mkdir \"" + cond.name + "\"";
            system(str.c_str());
            //создание папок
            while(nameTab!= nullptr){
                string allPapka = "cd \""+cond.name + "\" && mkdir \"" + nameTab->table + "\"";
                system(allPapka.c_str());

                string allCsv="cd \""+ cond.name + "\"/\""+ nameTab->table + "\" && touch 1.csv";
                system(allCsv.c_str());

                string allCol= nameTab->table + "_pk";
                while(nameCol!= nullptr){
                    allCol+= "," + nameCol->tableColon;
                    nameCol=nameCol->next;
                }
                string allNumber ="cd \""+ cond.name + "\"/\""+ nameTab->table + "\" && touch "+nameTab->table+"_pk_sequence.txt";
                system(allNumber.c_str());
                ofstream file(cond.name+ "/"+ nameTab->table +"/"+nameTab->table+"_pk_sequence.txt");
                file<<1;
                file.close();

                ofstream out(cond.name+ "/"+ nameTab->table +"/1.csv");
                out << allCol;
                out.close();
                nameTab = nameTab->next;
                if(nameTab!=nullptr){
                     nameCol = nameTab->fColon;
                }
            }
            break;
        }else if(DeleteOrNo=="2"){
            break;
        }else{
            cout<<"Введите 1 или 2\n";
            cin>>DeleteOrNo;
        }
    }
}

//Перевод из числа в строку
string intInStrok(int element){
    string strok;
    while(element!=0){
        strok = char('0'+element%10)+strok;
        element/=10;
    }
    return strok;
}
//Перевод из строки в число 
int StrokInInt(string element){
    int number=0;
    for (size_t i = 0; i < element.size(); i++) {
        int digit = (int) element[i] - (int) '0';
        number = number * 10 + digit;
    }
    return number;
}

//Проверка на то не нулевая ли первая строка в файле
bool emptyFileOrNo(configuration cond, string nameTabl, string newNuber){
    int checkLimit=0;
    ifstream file;
    file.open(cond.name+ "/"+ nameTabl +"/"+newNuber+".csv");
    string element;
    while(getline(file, element)){
        checkLimit++;
    }
    if(checkLimit!=0){
        return true;
    }
    return false;
}

void Insert(string comand, configuration cond){
    //Раздел
    istringstream Stream(comand);
    string element;
    //Лишние эл
    Stream>>element;
    Stream>>element;

    //Наша таблица
    string nameTabl;
    Stream>>nameTabl;

    
    ofstream out;
   //номер сsv
    int number_file=1;
    while(true){
     //Счетчик для лимита строк
        int checkLimit=0;
        string newNuber = intInStrok(number_file);
        ifstream file;
        file.open(cond.name+ "/"+ nameTabl +"/"+newNuber+".csv");
        while(getline(file, element)){
            checkLimit++;
        }

        file.close();
        if(checkLimit<cond.tuples_limit){
            out.open(cond.name+ "/"+ nameTabl +"/"+newNuber+".csv", ios_base::app);
            bool empty_file = emptyFileOrNo(cond, nameTabl, newNuber);
            if(empty_file){
                out <<'\n';
            }
            break;
        }
    number_file++;

}
    ifstream fileNumber;
    fileNumber.open(cond.name+ "/"+ nameTabl +"/"+nameTabl+"_pk_sequence.txt");
    element="";
    getline(fileNumber,element);
    fileNumber.close();
    //Номер строки, ключ для нее
    out<<element;
    //Добавочные элементы
    for(size_t i=0; comand[i]!='\0'; i++){
        string data="";
        if(comand[i]=='\''){
            for(size_t j=i+1; comand[j]!='\'';j++){
                data+=comand[j];
                i=j;
            }
            i+=2;
            out <<",";
            out <<data;
        }
    }
    out.close();
    ofstream newfileNumber;
    newfileNumber.open(cond.name+ "/"+ nameTabl +"/"+nameTabl+"_pk_sequence.txt", ios_base::out);
    int number= StrokInInt(element);
    number++;
    newfileNumber<<number;
    newfileNumber.close();
}

//Добавление новый индекс в структуру 
void AddStr(PrintColon* adding, string index){
    IndexPrintColon* newItem = new IndexPrintColon{index, nullptr}; 
    if(adding->strochki == nullptr){
        adding->strochki = newItem;
        return;
    }

    IndexPrintColon* goer = adding->strochki;
    while (goer->next != nullptr)
    {
        if(goer->indexFromStrok == index){
            return;
        }
        goer = goer->next;
    }
    if(goer->indexFromStrok == index){
        return;
    }
    goer->next = newItem;
    
}

//Возращаем в структуру
PrintColon* Select(string comand, string condName){
    PrintColon* chosen = new PrintColon;
    chosen->next = nullptr;
    chosen->strochki = nullptr;
    //Чтобы удобнее бежать
    PrintColon* inChosen = chosen;
    for(size_t i=6; i<comand.size();i++){
        
        string TablName="";
        string ColonName="";
        if(comand[i]!=' '&& i<comand.size()){

            if(i > 10){
                inChosen->next = new PrintColon;
                inChosen = inChosen->next;
                inChosen->next = nullptr;
                inChosen->strochki = nullptr;
            }
            //Таблица
            while(comand[i]!='.'  && i<comand.size()){
                TablName+=comand[i];
                i++;
            }
            i++;
            //Колонка
            while(comand[i]!=',' && comand[i]!=' ' && i<comand.size()){
                ColonName+=comand[i];
                i++;
            }
            int number_file=1;
            //Для поиска колонки
         
            //Прохожу по всем csv из табл
            while(true){
                int numberColon=0;
                int checkLimit=0;
                string newNuber = intInStrok(number_file);
                ifstream file;
                file.open(condName+ "/"+ TablName +"/"+newNuber+".csv");
                string element="";

                while(getline(file, element)){
                    checkLimit++;
                }
                if(checkLimit==0){
                    break;
                }   
                file.close();
                element="";
                //Номер колонки
                file.open(condName+ "/"+ TablName +"/1.csv");
                getline(file, element);
                string colonka="";
                for(size_t j=0; j<element.size(); j++){
                        if(element[j]!=','&& j<element.size()){
                                colonka+=element[j];
                        }else{
                                if(colonka==ColonName){
                                    break;
                                }else{
                                    numberColon++;
                                    colonka="";
                                }
                        }
                }
                file.close();
                
                element="";
                file.open(condName+ "/"+ TablName +"/"+newNuber+".csv");
                if(number_file == 1){
                    getline(file, element);
                }
                inChosen->fromTabl = TablName;
                inChosen->fromColon = numberColon;
                //Вывод колонок
                while(getline(file, element)){
                    element+=',';
                    colonka="";
                    int ourСolumn=0;
                    for(size_t j=0; j<element.size(); j++){
                        if(element[j]!=','){
                            colonka+=element[j];
                        }else{
                            if(ourСolumn==0){
                                //Сохраняем в структуре
                                 AddStr(inChosen, colonka);
                                colonka="";
                                break;
                            }else{
                                ourСolumn++;
                                colonka="";
                        }
                    }      
                }
            }
            number_file++;
            file.close();

            }

        }
    }
    return chosen;
}

//По индексу и номеру колонки ввыводит ячейку

string GetItem(string CondName, string TablName, string index, int colon){
    int number_file=1;
    //Для поиска колонки
    
    //Прохожу по всем csv из табл
    
    while(true){
        int checkLimit=0;
        string newNuber = intInStrok(number_file);
        ifstream file;
        file.open(CondName+ "/"+ TablName +"/"+newNuber+".csv");
        string element="";
        while(getline(file, element)){
            checkLimit++;
        }
        if(checkLimit==0){
            break;
        }   
        file.close();
        string colonka = "";        
        element="";
        file.open(CondName+ "/"+ TablName +"/"+newNuber+".csv");
        //Вывод колонок
        while(getline(file, element)){
            element+=',';
            colonka="";
            int ourСolumn=0;
            for(size_t j=0; j<element.size(); j++){
                if(element[j]!=','){
                    colonka+=element[j];
                }else{
                    //Есть ли в строке мой элемент
                    if(ourСolumn == 0 && colonka != index){
                        break;
                    }
                    //Та ли эта колонка
                    if(ourСolumn==colon){
                        return colonka;
                    }else{
                        ourСolumn++;
                        colonka="";
                    }
                }      
            }
        }
        number_file++;
        file.close();

    }
    return "";
}

//Обрезает строчку до From

string CutToFrom(string command){
    istringstream streamCommand(command);
    string newCommand = "";
    string s;
    while (streamCommand >> s){
        if(s != "FROM"){
            newCommand += s + ' ';
        }
        else{
            return newCommand;
        }
    }
    return newCommand;
}

//Отделяет условия
string CutAfterWhere(string command){
    istringstream streamCommand(command);
    string newCommand = "";
    string s;
    while (streamCommand >> s){
        if(s == "WHERE"){
            newCommand += s + ' ';
            break;
        }
    }
    while (streamCommand >> s){
        newCommand += s + ' ';
    }
    return newCommand;
}

//Вывод по индексам 
string StrPrint(string CondName, PrintColon* chosen){
    //Все индексы
    string buffer="";
    IndexPrintColon* inStr = chosen->strochki;
        while(inStr!=nullptr){
            string first = GetItem(CondName, chosen->fromTabl, inStr->indexFromStrok, chosen->fromColon);
            //Если 2 таблицы
            if(chosen->next != nullptr){
                PrintColon* inChosen = chosen->next;
                IndexPrintColon* inInStr = inChosen->strochki;
                while(inInStr!=nullptr){
                    string second = GetItem(CondName, inChosen->fromTabl, inInStr->indexFromStrok, inChosen->fromColon);
                    buffer+= first+' '+second+'\n';

                    inInStr = inInStr->next;
                }
            }
            //Если только 1 таблица
            else{
                buffer+= first+'\n';
            }
            inStr = inStr->next;
        }
    return buffer;
}

//Удаление индекса
void deleteIndex(PrintColon* chosen, string index){
    if(chosen->strochki == nullptr){
        return;
    }
    IndexPrintColon* str = chosen->strochki;
    //Смотрим первый элемент/ удл с головы
    if(str->indexFromStrok == index){
        chosen->strochki = str->next;
        delete str;
        return;
    }
    while(str->next != nullptr){
        if(str->next->indexFromStrok == index){
            IndexPrintColon* mus = str->next;
            str->next = str->next->next;
            delete mus;
            return;
        }
        str = str->next;
    }
}

// Для условия и удаление сравнение колонки со строкой
void colonAndStr(string CondName, PrintColon* chosen, string str, int colonNum){
    IndexPrintColon* cstr = chosen->strochki;
    while(cstr!=nullptr){
        if(GetItem(CondName, chosen->fromTabl, cstr->indexFromStrok, colonNum) != str){
            //Удаляем из списка индексов, чтобы потом вывести 
            deleteIndex(chosen, cstr->indexFromStrok);
            cstr = chosen->strochki;
        }
        else{
            cstr = cstr->next;

        }
    }
}

//Удаляет ковычки из строчки
string delKavich(string s){
    string newS = "";
    for(size_t i = 1; i < s.size() - 1; i++){
        newS += s[i];
    }
    return newS;
}

//Сравнение колонки с другой колонкой
void colonAndColon(string CondName, PrintColon* chosen, string tabl1, int colon1, string tabl2, int colon2){
    PrintColon* chosen1;
    PrintColon* chosen2;
    //Раскидываем таблиц
    if(chosen->fromTabl == tabl1){
        chosen1 = chosen;
    }
    
    if(chosen->next->fromTabl == tabl1){
        chosen1 = chosen->next;
    }
    
    if(chosen->fromTabl == tabl2){
        chosen2 = chosen;
    }
    
    if(chosen->next->fromTabl == tabl2){
        chosen2 = chosen->next;
    }

    IndexPrintColon* str1 = chosen1->strochki;
    //Добавляем только парные элементы
    PrintColon* paired1 = new PrintColon;
    PrintColon* paired2 = new PrintColon;
    while(str1 != nullptr){
        IndexPrintColon* str2 = chosen2->strochki;

        while (str2 != nullptr)
        {
            //Сравнение элементов
            if(GetItem(CondName,tabl1,str1->indexFromStrok,colon1) == GetItem(CondName, tabl2, str2->indexFromStrok, colon2)){
                AddStr(paired1, str1->indexFromStrok);
                AddStr(paired2, str2->indexFromStrok);
            }
            str2 = str2->next;
        }
        str1 = str1->next;

        
    }
    chosen1->strochki = paired1->strochki;
    chosen2->strochki = paired2->strochki;

}

//Соединяет 2 списка индексов для OR
void LinkChosens(PrintColon* adding, PrintColon* chosen2){
    if(adding->strochki == nullptr){
        adding->strochki = chosen2->strochki;
        return;
    }
    IndexPrintColon* goer = adding->strochki;
    while (goer->next != nullptr)
    {
        goer = goer->next;
    }
    goer->next = chosen2->strochki;
}

//Условия
void Where(string CondName, PrintColon* chosen, string comand, string adComand){
    if(comand == "") return;
    //PrintColon* wcolons = new PrintColon;
    istringstream whStream(comand);
    string temp;
    //Лишний
    whStream >> temp;
    whStream >> temp;
    string TablName="";
    string ColonName="";
    size_t i = 0;
    //Таблица
    while(temp[i]!='.'  && i<temp.size()){
        TablName+=temp[i];
        i++;
    }
    i++;
    //Колонка
    while(temp[i]!=' ' && i<temp.size()){
        ColonName+=temp[i];
        i++;
    }
    ifstream file;
    string element;
    int numberColon = 0;
    file.open(CondName+ "/"+ TablName +"/1.csv");
    getline(file, element);
    string colonka="";
    for(size_t j=0; j<element.size(); j++){
            if(element[j]!=','&& j<element.size()){
                    colonka+=element[j];
            }else{
                    if(colonka==ColonName){
                        break;
                    }else{
                        numberColon++;
                        colonka="";
                    }
            }
    }
    file.close();
    //=
    whStream >> temp;

    whStream >> temp;
    if(temp[0] == '\''){
        //Сравниваем со строкой
        if(chosen->fromTabl == TablName){
            colonAndStr(CondName, chosen, delKavich(temp), numberColon);
        }
        if(chosen->next != nullptr && chosen->next->fromTabl == TablName){
            colonAndStr(CondName, chosen->next, delKavich(temp), numberColon);
        }
    }
    else{
        string TablName2="";
        string ColonName2="";
        size_t z = 0;
        //Если идет сравнение с другой таблицей, а не со строкой
        while(temp[z]!='.'  && z<temp.size()){
            TablName2+=temp[z];
            z++;
        }
        z++;
        while(temp[z]!=' ' && z<temp.size()){
            ColonName2+=temp[z];
            z++;
        }
        element="";
        int numberColon2 = 0;
        file.open(CondName+ "/"+ TablName2 +"/1.csv");
        getline(file, element);
        colonka="";
        for(size_t j=0; j<element.size(); j++){
                if(element[j]!=','&& j<element.size()){
                        colonka+=element[j];
                }else{
                        if(colonka==ColonName2){
                            break;
                        }else{
                            numberColon2++;
                            colonka="";
                        }
                }
        }
        file.close(); 
        //Сравнение колонки с другой колонкой
        colonAndColon(CondName, chosen, TablName, numberColon, TablName2, numberColon2);
    }

    whStream >> temp;
    //Считываю условие 
    if(temp == "AND"){
        string newCommand = temp;
        while (whStream>>temp){
            newCommand += ' ' + temp;
        }
        //Профильтровать то что уже отфольтровали
        Where(CondName, chosen, newCommand, adComand);
    }
    if(temp == "OR"){
        string newCommand = temp;
        while (whStream>>temp){
            newCommand += ' ' + temp;
        }
        //щздадим новую сруктуру
        PrintColon* adChosen = Select(adComand, CondName);
        //Пройдем по следующим элементам
        Where(CondName, adChosen, newCommand, adComand);
        //Соединим индексы
        LinkChosens(chosen, adChosen);

    }
}

//Удаление по индексу
void Delete(string condName, string TablName, string index){
    int number_file = 1;
    while(true){
        string newNuber = intInStrok(number_file);
        ifstream file;
        file.open(condName+ "/"+ TablName +"/"+newNuber+".csv");
        string element="";
        string text = "";
        string ind = "";
        getline(file, element);
        for(size_t i = 0; element[i] != ','; i++){
            ind+= element[i];
        }
        if(ind != index){
            text = element;
        }
        //Записываем в text все кроме строки с нашим индексом
        while(getline(file, element)){
            string inds = "";
            for(size_t i = 0; element[i] != ','; i++){
                inds += element[i];
            }
            if(inds != index){
                text += '\n' + element;
            }
        }   
        file.close();

        ofstream fileOut;
        fileOut.open(condName+ "/"+ TablName +"/"+newNuber+".csv", ios_base::out);
        //Перезаписываем содержиое файла
        fileOut << text;
        if(text == "") break;

        number_file++;

    }
}

//Удаление нескольких индексов
void StrDel(string CondName, PrintColon* chosen){
    IndexPrintColon* strs = chosen->strochki;
    while(strs != nullptr){
        Delete(CondName, chosen->fromTabl, strs->indexFromStrok);
        strs = strs->next;
    }
}

//Блокировка файла
void LockTable(string CondName){
    ofstream file;
    file.open(CondName + "/" + CondName + "_lock.txt", ios_base::out);
    file.clear();
    file << "заблокирован";
}
//Разблок
void UnlockTable(string CondName){
    ofstream file;
    file.open(CondName + "/" + CondName + "_lock.txt", ios_base::out);
    file << "разблкирован";
}

bool isLocked(string CondName){
    ifstream file;
    file.open(CondName + "/" + CondName + "_lock.txt", ios_base::out);
    string element = "";
    getline(file, element);
    if(element == "заблокирован"){
        return 1;
    }
    return 0;

}

//Прошлый main , мой клманды
string Commands(string comand, configuration cond){
     //Раздел
    istringstream Stream(comand);
    string element;
    //Для обработки команды
    string buffer="";
    while(Stream>>element){

    if(element=="INSERT"){
        Insert(comand, cond);
    }else if(element=="SELECT"){
        string selCommand = CutToFrom(comand);
        string wheCommand = CutAfterWhere(comand);
        PrintColon* chosen = Select(selCommand, cond.name);
        Where(cond.name, chosen, wheCommand, selCommand);
        buffer += StrPrint(cond.name, chosen);
    }else if (element == "DELETE")
    {
        Stream>>element;
        Stream>>element;
        string tablName = element;
        string wheCommand = CutAfterWhere(comand);
        //На основе удаления создаем селект
        string selCommand = "SELECT " + tablName + "." + tablName + "_pk";
        PrintColon* chosen = Select(selCommand, cond.name);
        Where(cond.name, chosen, wheCommand, selCommand);

        StrDel(cond.name, chosen);
    }
    else if (element == "EXIT"){
        UnlockTable(cond.name);
        exit(0);
    }
    
    
}
    return buffer;
}

//Функции из сервера
bool ExitOrNO(const char* meaning){
    for(size_t i=0; i< strlen(meaning); ++i){
        if(meaning[i] == SYMBOL){
            return true;
        }
    }
    return false;
 }

void ErrorOrNo(int meaning){
    if(meaning<0){
        cout<<"Error\n";
        exit(0);
    }
 }

void ClientOrServer(char* buffer){
    if(ExitOrNO(buffer)){
        cout<<"Goodbye... \n";
    }
}

//Отправление сообщения
 void sendAMessage(int system, char* buffer, configuration cond){
    //Проверка не символ ли
     if(ExitOrNO(buffer)){
        cout<<"Goodbye... \n";
        return;
    }
    //Новый код
    string str="";
    //перенесла в строку
    for(size_t i=0; i<strlen(buffer);i++){
        str += buffer[i];
    }
    string result = Commands(str, cond)+'\0';
    //Обратно
    for(size_t i=0; i< result.length();i++){
        buffer[i]=result[i];
    }
    //Отправляет сообщение  сервису
    send(system, buffer, MAX_SIZE_BUF,0);
    
 }

 //Принять сообщение
 void AcceptTheMessage(int system, char* buffer){
     recv(system, buffer, MAX_SIZE_BUF, 0);
    //Проверка не символ ли
   ClientOrServer(buffer);

 }

 //Связь с клиентом
 void Communication_With_Client(int server, configuration cond){
    bool isExit = false;
    char buffer[MAX_SIZE_BUF];
        //Копирует сообщение в буфер.
        strcpy(buffer, "=> Server connected!\n");

        //Отправляет сообщение клиенту.
        send(server, buffer, MAX_SIZE_BUF, 0);
        cout<<"=> Connected to the client "<<endl<<"Enter "<<SYMBOL<<" to end the connection\n\n";
        //Если клиент не завершил
        while(!isExit){
            AcceptTheMessage(server, buffer);
           if(ExitOrNO(buffer)){
                break;
            }
            g_lock.lock();
            cout<<"Client № "<<server-3<<" : "<<endl;
             cout<<buffer<<endl;
            cout<<"Server : ";
            sendAMessage(server, buffer, cond);
            cout<<"Запрос выплне"<<endl;
            g_lock.unlock();
             
        }

        //cout<<"\n GoodBye..."<<endl;
        //isExit=false;
 }
