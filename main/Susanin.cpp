#include "Susanin.hpp"


float estimate_time(const std::chrono::steady_clock::duration &start_time_duration)
{
    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto time = end_time.time_since_epoch() - start_time_duration;
    return std::chrono::duration<float>(time).count();
}



Susanin::Susanin(){
    this->ds_file_name = "test_input.txt";
};
Susanin::~Susanin(){};
//done
int Susanin::modeInputHandler(){
    curs_set(0);
    id_t chosen_mode = 0;
    const unsigned int modes_count = 6;
    const std::string modes[modes_count] = {
        " 1. Среди кратчайших по времени путей между двумя городами найти путь минимальной стоимости",
        " 2. Среди путей между двумя городами найти путь минимальной стоимости",
        " 3. Среди путей между двумя городами найти путь минимальный по числу посещенных городов",
        " 4. Найти множество городов (и минимальных по стоимости путей к ним), достижимых из города отправления  не более чем за обозначенную сумму денег",
        " 5. Найти множество городов (и минимальных по времени путей к ним), достижимых из города отправления не более чем за обозначенное время",  
        "Завершить программу"};
    int user_choice;
    bool is_proccessing = true;

    // clear();
    // refresh();

    mvprintw(0, 0, "Добро пожаловать! Выберите режим работы");

    while (is_proccessing)
    {

        for (int i = 0; i < modes_count; i++){
            if (chosen_mode == i){
                attron(A_REVERSE);
            }
            mvprintw(i + TITLE_STEP, 0, modes[i].c_str());
            attroff(A_REVERSE);
            
            /*
            switch(i){
                case 4:
                    //mvprintw(i + 3, 0, modes[i].c_str());
                    break;
                case 5:
                    attron(COLOR_PAIR(5));
                    mvprintw(i + 8, 0, modes[i].c_str());
                    attroff(COLOR_PAIR(5));
                default:
                    mvprintw(i+2, 0, modes[i].c_str());
                    break;
            }
            attroff(A_REVERSE);*/
        }

        user_choice = getch();
        //обработка клавиш
        switch (user_choice){
        case KEY_UP:
            chosen_mode = (chosen_mode == 0) ?(modes_count - 1):(chosen_mode-1); 
            break;
        case KEY_DOWN:
            chosen_mode = (chosen_mode == modes_count - 1)?0:(chosen_mode+1); 
            break;
        case 10:
            is_proccessing = false;
        default:
            break;
        }
    }

    return (chosen_mode != 5)?chosen_mode:-1;
};



//done
std::string Susanin::intInputhandler(){
    echo();
    char input[200];
    attron(COLOR_PAIR(4));
    getnstr(input, 200);
    attroff(COLOR_PAIR(4));
    std::string str(input);
    noecho();
    return str;
};

//done
int Susanin::finishAppHandler(){
    curs_set(0);
    const unsigned int modes_count = 2;
    int chosen_mode = 0;
    const std::string modes[modes_count] = {
        " 1. Завершить программу",
        " 2. Вернутся на страницу выбора режима работы"};
    int user_choice;
    bool is_proccessing = true;

    mvprintw(0, 0, "Выберите режим работы");

    while (is_proccessing){

        for (int i = 0; i < modes_count; i++){
            if (chosen_mode == i){
                attron(A_REVERSE);
            }
            mvprintw(i + TITLE_STEP, 0, modes[i].c_str());
            attroff(A_REVERSE);
        }
        user_choice = getch();

        switch (user_choice){
        case KEY_UP:
            chosen_mode = (chosen_mode == 0)?(modes_count - 1):(chosen_mode-1);
            break;
        case KEY_DOWN:
            chosen_mode = (chosen_mode == modes_count - 1)?0:(chosen_mode+1);
            break;
        default:
            break;
        }

        if (user_choice == 10){
            return chosen_mode;
        }
    }

    return chosen_mode;
};

bool Susanin::addRestrictedTransportHandler(SearchAlgorithm&  s){
    return false;
}

/*
this->ds_file.open(ds_name.c_str());
        if (this->ds_file.fail()){
            std::cout<<"Ошибка с открытием датасета"<<std::endl;
        }
*/
//вроде done 
id_t Susanin::getCityHandler(Graph &graph, bool to_city){
    curs_set(1);
    if (to_city){
        printw("Введите город прибытия");    
    }
    else{
    printw("Введите город отправления");
    }

    move(1, 0);

    refresh();

    std::string city_name = this->stringInputHandler(1);
    id_t city_from = 0;
    bool validation_input = true;
    while(validation_input){
        try{
            city_from = graph.findCityByName(city_name);
            validation_input = false;
        }
        catch(std::domain_error e){
            this->clear_n_lines(3, 4);
            mvprintw(3, 0, "Введите корректный город");
            move(4, 0);
            city_name = this->stringInputHandler(4);
            if (city_name == "EXIT_APP"){
                //доделать выход по исключению
                this->endLoop(); 
            }    
        }
    }

    this->clear_n_lines(0, 4);

    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));
    mvprintw(2, 0, "Ваш город: ");
    attron(COLOR_PAIR(2));
    mvprintw(2, 23, city_name.c_str());
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));
    mvprintw(4, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));

    this->wait_for_enter(6);

    return city_from;
}

std::string Susanin::formatString(const std::string &input){

    std::string result = "";
    for (const auto &c : input)
    {
        if (c != '"')
        {
            result += c;
        }
    }
    return result;
}

std::string Susanin::stringInputHandler(int row){
    int c;
    std::string str = "";
    std::string new_typed = "";
    int i = 0;
    bool backspace_tap_after_input = true;
    while ((c = getch()) != 10)
    {
        if (c != -1 && (c < 258 || c > 261))
        {
            if (c == 127 || c == KEY_BACKSPACE)
            {
                if (backspace_tap_after_input)
                {
                    i -= ((new_typed.length() / 2));
                    backspace_tap_after_input = false;
                    new_typed.clear();
                }

                if (i > 0)
                {
                    if (str.back() < '0')
                    {
                        str.pop_back();
                        str.pop_back();
                    }
                    else
                    {
                        str.pop_back();
                    }
                    i -= 1;
                    move(row, i);
                }
                else
                {
                    i = 0;
                }
                clrtoeol();
            }
            else if (c == ctrl(27))
            {
                return EXIT_APP;
            }
            else if (char(c) < '0')
            {
                backspace_tap_after_input = true;
                attron(COLOR_PAIR(4));
                printw("%c", c);
                attroff(COLOR_PAIR(4));
                str.push_back(char(c));
                new_typed.push_back(char(c));
                i++;
            }
            else
            {
                backspace_tap_after_input = true;
                attron(COLOR_PAIR(4));
                printw("%c", c);
                attroff(COLOR_PAIR(4));
                str.push_back(char(c));
                new_typed.push_back(char(c));
                new_typed.push_back(char(c));
                i += 2;
            }
        }
    }
    return str;
}

//done
void Susanin::readGraph(Graph &graph){
    
    std::string result;
    std::ifstream input(this->ds_file_name.c_str());

    while (getline(input, result)){
        std::string item;
        std::string cityPart;
        std::stringstream ss(result);
        std::size_t iter = 0;

        id_t from_city_id;
        id_t to_city_id;
        id_t transport;
        weight_t time;
        weight_t fare;

        while (getline(ss, item, ' ')){
            if (item != "\n"){

                if (item[0] == '"' && item.back() != '"'){
                    cityPart = item;
                    continue;
                }
                if (cityPart != ""){
                    item = cityPart + ' ' + item;
                    cityPart = "";
                }
                // const auto arr = item.c_str();
                // item.erase(remove(arr.begin(), arr.end(), '"'), arr.end());
                item = this->formatString(item);

                switch (iter){
                case 0:
                    from_city_id = graph.addCity(item);
                    break;
                case 1:
                    to_city_id = graph.addCity(item);
                    break;
                case 2:
                    transport = graph.addTransport(item);
                    break;
                case 3:
                    time = stoi(item);
                    break; 
                case 4:
                    fare = stoi(item);
                    graph.addFlight(from_city_id, to_city_id, transport, time, fare, 1);
                    break;
                }
                iter++;
            }
        }
    } 

    input.close();
}

void Susanin::beginLoop(){
    setlocale(LC_ALL, "ru_RU.UTF-8");
    Graph graph = Graph();
    //auto logger = Logger(graph);
    if ( !this->ds_file_name.length()){
        std::cout << "Введите название файла с билетами" << std::endl;
        std::cin >>this->ds_file_name;     
    }

    this->readGraph(graph);

    initscr();
    noecho();
    cbreak();
    halfdelay(5);

    // scrollok(stdscr, true);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, 10, COLOR_BLACK);
    init_pair(3, 13, COLOR_BLACK);
    init_pair(4, 15, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
};

void Susanin::endLoop(){
    clear();
    refresh();
    endwin();
};

int Susanin::loop(){
    double total_time;
    
    setlocale(LC_ALL, "ru_RU.UTF-8");
    // блок инициализации
    Graph graph = Graph();
    //auto logger = Logger(graph);
    if ( !this->ds_file_name.length()){
        std::cout << "Введите название файла с билетами" << std::endl;
        std::cin >>this->ds_file_name;     
    }

    this->readGraph(graph);

    initscr();
    noecho();
    cbreak();
    halfdelay(5);

    // scrollok(stdscr, true);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, 10, COLOR_BLACK);
    init_pair(3, 13, COLOR_BLACK);
    init_pair(4, 15, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    // конец начала 
    keypad(stdscr, true);

    bool proccess_in_interaction = true;

    while (proccess_in_interaction){

        const int result = this->modeInputHandler();

        clear();
        refresh();
        if (result == -1){
            break;
        }
        //получаем город отравления
        id_t from = this->getCityHandler(graph, false);
        clear();
        refresh();
        // если путь куда-то, то берем 2 город
        id_t to;
        if(result < 3){
            to = this->getCityHandler(graph, true);
            clear();
            refresh();
        }

        //получить ограничения на транспорт
        Dijkstra sercher = Dijkstra(graph);
         
        if (!this->addRestrictedTransportHandler(sercher)){
            std::cout<<"FALSE addRestrictedTransport"<<std::endl;
        };
        //const weights_t limits = this->get_
        
        clear();
        refresh();

        switch (result){
            //среди кратчайших по времени минимальный по стоимости
            case 0:{
                const auto start_time = std::chrono::high_resolution_clock::now();
                //запуск поиска
                try{
                    const auto path = sercher.findFastRoute(to, from);
                    
                    total_time = estimate_time(start_time.time_since_epoch());

                    printw("Искомый путь: ");
                    attron(COLOR_PAIR(4));
                    printw("тут путь: ");
                    //mvprintw(2, 0, path_string.c_str());
                    attroff(COLOR_PAIR(4));    
                }
                catch (std::runtime_error& e){
                    total_time = estimate_time(start_time.time_since_epoch());
                    printw("Пути нет");
                }
                catch (std::domain_error& e){
                    total_time = estimate_time(start_time.time_since_epoch());
                    printw("Город отправления = городу прибытия");    
                }

                const auto log_title = "Путь минимальной стоимости среди кратчайших по времени";
                const auto city_from_title = "Город отправления: " + graph.findCityById(from) ;
                const auto city_to_title = "Город прибытия: " + graph.findCityById(to);
                
                std::cout<<city_from_title<<city_to_title<<std::endl;
                break;
            }
            //путь минимальной стоимости 
            case 1:{
                break;
            }
            //путь минимальный по числу посещенных городов
            case 2:{
                break;
            }
            case 3:
                break;
            case 4:
                break;
            //завершить программу эт -1
            default:
                std::cout<<"ЧТО-ТО ПОШЛО не туда!!!"<<std::endl;
                break;
        }
        // refresh();
        
        //ffkfkfkfkfkfkfkfkfkfkfk

        char c;

        while ((c = getch()) != 10)
        {
            // break;
        }
        // wait_for_enter();

        clear();
        refresh();

        const auto chosen_mode = this->finishAppHandler();
        if (chosen_mode == 0)
        {
            proccess_in_interaction = false;
        }
    }
    //конец цикла
    // curs_set(0);
    endwin();
    return 0;
}
