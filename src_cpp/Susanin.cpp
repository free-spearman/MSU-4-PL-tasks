#include "Susanin.hpp"


float estimate_time(const std::chrono::steady_clock::duration &start_time_duration)
{
    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto time = end_time.time_since_epoch() - start_time_duration;
    return std::chrono::duration<float>(time).count();
}

Susanin::Susanin():ProcessLogger(typeid(this).name()){
    this->beginLogMess();
};


Susanin::Susanin(const char *ds_name):ProcessLogger(typeid(this).name()){
    this->ds_file_name = ds_name;
    this->beginLogMess();
    this->log(this->ds_file_name.c_str(), "Susanin init");
};

void Susanin::setDS(){
    std::cout<<"Введите имя файла с билетами"<<std::endl;
    std::cin>>this->ds_file_name;
    std::cout<<"Вы ввели:"<<this->ds_file_name<<"\n Начинаю рабоут ..."<<std::endl;
    this->log(this->ds_file_name.c_str(), "Susanin setDS");
};

void Susanin::setDS(const char *ds_name){
    this->ds_file_name = ds_name;
    this->log(this->ds_file_name.c_str(), "Susanin setDS");
};

Susanin::~Susanin(){};

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
    scrollok(stdscr, 1);
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

weight_t Susanin::RestrictedWeightsHandler(const char *mess){
    clear();
    refresh();

    curs_set(1);
    std::string out_str = "Введите ограничение по ";
    out_str+= mess;
    printw(out_str.c_str());
    
    move(1, 0);

    refresh();
    bool validation_input = true;
    std::string limit_input;
    weight_t limit;
    while (validation_input){
        limit_input = this->intInputhandler();
        try{
            limit = std::stoul(limit_input);
            validation_input = false;
        }
        catch(std::invalid_argument){
            clear_n_lines(3, 4);
            mvprintw(3, 0, "Число введено некорректно, повторите попытку");
            move(4, 0);
        }
        catch(std::out_of_range){
            clear_n_lines(3, 4);
            mvprintw(3, 0, "Число слишком большое, повторите попытку");
            move(4, 0);
        }
        
    }

    clear_n_lines(0, 4);

    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));
    out_str = "Введено ограничение: " + limit_input + "у. е.";   

    if (limit == 0){
        attron(COLOR_PAIR(2));
        mvprintw(2, 0, "Нет ограничения");
        attroff(COLOR_PAIR(2));
        limit = std::numeric_limits<weight_t>::max();
    }
    else{
    attron(COLOR_PAIR(2));
    mvprintw(2, 0, out_str.c_str());
    attroff(COLOR_PAIR(2));
    }

    attron(COLOR_PAIR(3));
    mvprintw(4, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));

    

    this->wait_for_enter(6);

    return limit;
};

int Susanin::loop(){
    double total_time;
    this->log("Запуск программы", "loop");
    this->startLog("RUN");

    setlocale(LC_ALL, "ru_RU.UTF-8");
    // блок инициализации
    Graph graph = Graph();
    //auto logger = Logger(graph);
    if ( !this->ds_file_name.length()){
        std::cout << "Введите название файла с билетами" << std::endl;
        std::cin >>this->ds_file_name;     
    }
    this->startLog("readGraph");
    this->readGraph(graph);
    this->endLog();
    initscr();
    noecho();
    cbreak();
    halfdelay(5);

    scrollok(stdscr, true);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, 10, COLOR_BLACK);
    init_pair(3, 13, COLOR_BLACK);
    init_pair(4, 15, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    // конец начала 
    keypad(stdscr, true);

    bool working = true;

    Dijkstra sercher = Dijkstra(graph);

    while (working){

        const int result = this->modeInputHandler();

        clear();
        refresh();
        if (result == -1){
            break;
        }
        //this->log("clear", "запрос пользователя");
        this->startLog("Запрос пользователя");
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
        sercher.resetRestrictionsTransport();
        while(this->addRestrictedTransportHandler(sercher) != 0);
        this->log(sercher.getProhibitedTransport().c_str(), "RestrictedTransportHandler"); 
        //
        weights_t limits;
        limits[CRUISE_FARE_P] = std::numeric_limits<weight_t>::max();
        limits[NUM_LOCALS_P] = std::numeric_limits<weight_t>::max();
        limits[CRUISE_TIME_P] = std::numeric_limits<weight_t>::max();
        if (result == 3){
            limits[CRUISE_FARE_P] = this->RestrictedWeightsHandler("цене\n");    
        }
        if (result == 4){
            limits[CRUISE_TIME_P] = this->RestrictedWeightsHandler("времени\n");
        }

        
        std::string weights_log = std::to_string(limits[CRUISE_TIME_P]) + std::to_string(limits[CRUISE_FARE_P]) + std::to_string(limits[NUM_LOCALS_P]);
        this->log( weights_log.c_str(), "ограничения");

        sercher.setLimits(limits);
        //
        clear();
        refresh();
        this->log("До switch", "loop");
        switch (result){
            //среди кратчайших по времени минимальный по стоимости
            case 0:{
                //запуск поиска
                this->startLog("case0");
                try{
                    const auto log_title = "Путь минимальной стоимости среди кратчайших по времени";
                    //const auto city_from_title = "id:"+ std::to_string(from)+ " name:" + graph.findCityById(from);
                    //const auto city_to_title = "id:"+ std::to_string(to)+ " name:" + graph.findCityById(to);
                    //this->log(city_from_title.c_str(), "city_from_title");
                    //this->log(city_to_title.c_str(), "city_to_title");
                    //this->log(graph.findCityById(from),"findFastRoute:from");
                    const auto path = sercher.findFastRoute(from, to);
                    this->endLog();
                    
                    //printw("Искомый путь: \n");
                    attron(COLOR_PAIR(4));
                    std::string path_strign = graph.routeToString(path);

                    //const auto log_title = "Путь минимальной стоимости среди кратчайших по времени";
                    //const auto city_from_title = "Город отправления: " + graph.findCityById(from) ;
                    //const auto city_to_title = "Город прибытия: " + graph.findCityById(to);
                    printw(log_title);
                    this->log(path_strign.c_str(), "findFastRoute::");
                    mvprintw(3, 0, path_strign.c_str());

                    attroff(COLOR_PAIR(4));    
                }
                catch (std::runtime_error& e){
                    this->endLog();
                    printw("Пути нет");
                }
                catch (std::domain_error& e){
                    this->endLog();
                    printw("Город отправления = городу прибытия");    
                }
                
                //std::cout<<city_from_title<<city_to_title<<std::endl;
                break;
            }
            //путь минимальной стоимости 
            case 1:{
                this->startLog("case1");
                try{
                    const auto log_title = "Путь минимальной стоимости";
                    //const auto city_from_title = "id:"+ std::to_string(from)+ " name:" + graph.findCityById(from);
                    //const auto city_to_title = "id:"+ std::to_string(to)+ " name:" + graph.findCityById(to);
                    //this->log(city_from_title.c_str(), "city_from_title");
                    //this->log(city_to_title.c_str(), "city_to_title");
                    //this->log(graph.findCityById(from),"findFastRoute:from");
                    const auto path = sercher.findCheapRoute(from, to);
                    this->endLog();
                    
                    //printw("Искомый путь: \n");
                    attron(COLOR_PAIR(4));
                    std::string path_strign = graph.routeToString(path);

                    //const auto log_title = "Путь минимальной стоимости среди кратчайших по времени";
                    //const auto city_from_title = "Город отправления: " + graph.findCityById(from) ;
                    //const auto city_to_title = "Город прибытия: " + graph.findCityById(to);
                    printw(log_title);
                    this->log(path_strign.c_str(), "findCheapRoute::");
                    mvprintw(3, 0, path_strign.c_str());

                    attroff(COLOR_PAIR(4));    
                }
                catch (std::runtime_error& e){
                    this->endLog();
                    printw("Пути нет");
                }
                catch (std::domain_error& e){
                    this->endLog();
                    printw("Город отправления = городу прибытия");    
                }
                break;
            }
            //путь минимальный по числу посещенных городов
            case 2:{
                this->startLog("case2");
                try{
                    const auto log_title = "Путь минимальный по числу посещенных городов";
                    //const auto city_from_title = "id:"+ std::to_string(from)+ " name:" + graph.findCityById(from);
                    //const auto city_to_title = "id:"+ std::to_string(to)+ " name:" + graph.findCityById(to);
                    //this->log(city_from_title.c_str(), "city_from_title");
                    //this->log(city_to_title.c_str(), "city_to_title");
                    //this->log(graph.findCityById(from),"findFastRoute:from");
                    const auto path = sercher.findShortRoute(from, to);
                    this->endLog(); 
                    //printw("Искомый путь: \n");
                    attron(COLOR_PAIR(4));
                    std::string path_strign = graph.routeToString(path);

                    //const auto log_title = "Путь минимальной стоимости среди кратчайших по времени";
                    //const auto city_from_title = "Город отправления: " + graph.findCityById(from) ;
                    //const auto city_to_title = "Город прибытия: " + graph.findCityById(to);
                    printw(log_title);
                    this->log(path_strign.c_str(), "findShortRoute::");
                    mvprintw(3, 0, path_strign.c_str());

                    attroff(COLOR_PAIR(4));    
                }
                catch (std::runtime_error& e){
                    this->endLog();
                    printw("Пути нет");
                }
                catch (std::domain_error& e){
                    this->endLog();
                    printw("Город отправления = городу прибытия");    
                }
                break;
            }
            case 3:
                this->log(std::to_string(result).c_str(), "case3");
                try{
                    auto log_title = "Мно-во достижимых из города отправления  не более чем за обозначенную сумму денег\n";
                    //const auto city_from_title = "id:"+ std::to_string(from)+ " name:" + graph.findCityById(from);
                    //const auto city_to_title = "id:"+ std::to_string(to)+ " name:" + graph.findCityById(to);
                    //this->log(city_from_title.c_str(), "city_from_title");
                    //this->log(city_to_title.c_str(), "city_to_title");
                    //this->log(graph.findCityById(from),"findFastRoute:from");
                    this->startLog("достижимых из города отправления  не более чем за обозначенную сумму денег");
                    auto paths = sercher.findSetCitiesLimCost(from);
                    this->endLog(); 
                    //printw("Искомый путь: \n");
                    attron(COLOR_PAIR(4));
                    printw(log_title);
                    if (paths.size() >= 1){

                        for (auto path: paths){
                            std::string path_strign = graph.routeToString(path);    
                            //this->log(path_strign.c_str(), "findShortRoute::");
                            std::string city_to_title = graph.findCityById(path.get_to()) + "\n";
                            this->log(path_strign.c_str(), city_to_title.c_str());
                            printw(city_to_title.c_str());
                            printw(path_strign.c_str());
                            printw("\n");
                        }
                    }    
                    else {
                        printw("Путей нет");
                    }
                    attroff(COLOR_PAIR(4));

                    //const auto log_title = "Путь минимальной стоимости среди кратчайших по времени";
                    //const auto city_from_title = "Город отправления: " + graph.findCityById(from) ;
                    //const auto city_to_title = "Город прибытия: " + graph.findCityById(to);
                    
                        
                }
                    //attroff(COLOR_PAIR(4));

                    //const auto log_title = "Путь минимальной стоимости среди кратчайших по времени";
                    //const auto city_from_title = "Город отправления: " + graph.findCityById(from) ;
                    //const auto city_to_title = "Город прибытия: " + graph.findCityById(to);
                    
                 
                catch (std::runtime_error& e){
                    this->endLog();
                    printw("Пути нет");
                }
                catch (std::domain_error& e){
                    this->endLog();
                    printw("Город отправления = городу прибытия");    
                }
                break;
            case 4:
                this->log(std::to_string(result).c_str(), "case4");
                this->startLog("Мно-во достижимых из города отправления  не более чем за обозначенное время");
                try{
                    auto log_title = "Мно-во достижимых из города отправления  не более чем за обозначенное время\n";
                    //const auto city_from_title = "id:"+ std::to_string(from)+ " name:" + graph.findCityById(from);
                    //const auto city_to_title = "id:"+ std::to_string(to)+ " name:" + graph.findCityById(to);
                    //this->log(city_from_title.c_str(), "city_from_title");
                    //this->log(city_to_title.c_str(), "city_to_title");
                    //this->log(graph.findCityById(from),"findFastRoute:from");
                    auto paths = sercher.findSetCitiesLimTime(from);
                    this->endLog(); 
                    //printw("Искомый путь: \n");
                    attron(COLOR_PAIR(4));
                    printw(log_title);
                    if (paths.size() >= 1){

                        for (auto path: paths){
                            std::string path_strign = graph.routeToString(path);    
                            //this->log(path_strign.c_str(), "findShortRoute::");
                            std::string city_to_title = graph.findCityById(path.get_to()) + "\n";
                            this->log(path_strign.c_str(), city_to_title.c_str());
                            printw(city_to_title.c_str());
                            printw(path_strign.c_str());
                            printw("\n");
                        }
                    }
                    else {
                        printw("Путей нет");
                    }
                    attroff(COLOR_PAIR(4));

                    //const auto log_title = "Путь минимальной стоимости среди кратчайших по времени";
                    //const auto city_from_title = "Город отправления: " + graph.findCityById(from) ;
                    //const auto city_to_title = "Город прибытия: " + graph.findCityById(to);
                    
                        
                }
                catch (std::runtime_error& e){
                    this->endLog();
                    printw("Путей нет");
                }
                catch (std::domain_error& e){
                    this->endLog();
                    printw("Город отправления = городу прибытия");    
                }

                break;
            //завершить программу эт -1
            default:
                this->log("default case", "loop::switch");
                //std::cout<<"ЧТО-ТО ПОШЛО не туда!!!"<<std::endl;
                break;
        }
        // refresh();
        
        //ffkfkfkfkfkfkfkfkfkfkfk
        this->endLog();
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
            working = false;
        }
    }
    //конец цикла
    // curs_set(0);
    endwin();
    this->endLog();
    return 0;
}

//done
int Susanin::modeInputHandler(){
    this->log("Начало модуля", "modeInputHandler");
    clear();
    refresh();
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

        for (int i = 0, poss = 0; i < modes_count; i++, poss+=2){
            if (chosen_mode == i){
                attron(A_REVERSE);
            }

            mvprintw(poss + TITLE_STEP, 0, modes[i].c_str());
            attroff(A_REVERSE);
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

    int res = (chosen_mode != 5)?chosen_mode:-1;
    std::string log_str = "Был выбран:" + std::to_string(res);
    this->log(log_str.c_str() , "modeInputHandler");

    return res;
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

int Susanin::addRestrictedTransportHandler(SearchAlgorithm&  s){

    clear();
    refresh();

    curs_set(1);

    printw("Введите запрещенный транспорт или '.'");

    //this->log("printw()", "getCityHandler");

    move(1, 0);

    //this->log("move", "getCityHandler");
    
    
    std::string tr_name = this->stringInputHandler(1);
    
    bool validation_input = true;
    while(validation_input){
        try{
            s.addRestrictionsTransport(tr_name);
            validation_input = false;
        }
        catch(std::domain_error e){

            this->clear_n_lines(3, 4);
            if (tr_name == "."){
                return 0; 
            }
            mvprintw(3, 0, "Введите корректный транспорт");
            move(4, 0);
            tr_name = this->stringInputHandler(4);
            if (tr_name == "EXIT_APP"){
                //доделать выход по исключению
                this->endLoop(); 
            }    
        }
    }

    this->clear_n_lines(0, 4);

    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
    tr_name = "Ислючен:" + tr_name + "\n"; 
    mvprintw(2, 0, tr_name.c_str());
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(3));
    mvprintw(4, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));

    this->wait_for_enter(6);

    return 1;
};

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
    this->log("printw()", "getCityHandler");

    move(1, 0);

    //this->log("move", "getCityHandler");
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
    for (const auto &c : input){
        if (c != '"'){
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
    while ((c = getch()) != 10){
        if (c != -1 && (c < 258 || c > 261)){
            if (c == 127 || c == KEY_BACKSPACE){
                
                if (backspace_tap_after_input){
                    i -= ((new_typed.length() / 2));
                    backspace_tap_after_input = false;
                    new_typed.clear();
                }

                if (i > 0){
                    if (str.back() < '0'){
                        str.pop_back();
                        str.pop_back();
                    }
                    else{
                        str.pop_back();
                    }
                    i -= 1;
                    move(row, i);
                }
                else{
                    i = 0;
                }
                clrtoeol();
            }
            else if (c == ctrl(27)){
                return EXIT_APP;
            }
            else if (char(c) < '0'){
                backspace_tap_after_input = true;
                attron(COLOR_PAIR(4));
                printw("%c", c);
                attroff(COLOR_PAIR(4));
                str.push_back(char(c));
                new_typed.push_back(char(c));
                i++;
            }
            else{
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


