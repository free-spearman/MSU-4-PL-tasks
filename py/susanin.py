from py.graph import Graph
from py.flight import Flight
from py.logger import BaseLogger
import curses as crs
from py.serche_algorithm import SearchAlgorithm, Dijkstra
from py.custom_types import MAX_WEIGHT
from py.custom_types import weights_t 

EXIT_APP = "EXIT_APP"
TITLE_STEP = 2
def ctrl(x): return x & 0x1F

def read_graph_ds(filename: str, graph: Graph) -> None:
    ds_f = open(filename, 'r')

    for line in ds_f:
        iter = 0
        city_part = ''
        from_city_id = 0
        to_city_id = 0
        transport_id = 0
        time = 0
        fare = 0

        for item in line.split(' '):
            if item != '\n':
                if item[0] == '"' and item[-1] != '"':
                    city_part = item
                    continue

                if city_part != "":
                    item = city_part + ' ' + item
                    city_part = ""

                item = item.replace('"', '')

                if iter == 0:
                    from_city_id = graph.add_city(item)

                elif iter == 1:
                    to_city_id = graph.add_city(item)

                elif iter == 2:
                    transport_id = graph.add_transport(item)

                elif iter == 3:
                    time = int(item)

                elif iter == 4:
                    fare = int(item)
                    graph.add_flight(from_city_id, to_city_id, transport_id, time, fare)
                iter += 1

class Susanin:
    def __init__(self, ds_file_name, crswin : 'crs._CursesWindow'):
        self.crswin = crswin
        crs.noecho()
        crs.cbreak()
        crswin.scrollok(True)
        #self.locale = crs.setlocale(crs.LC_ALL, "ru_RU.UTF-8")
        # cs.halfdelay(5)
        crs.start_color()
        crs.init_pair(1, crs.COLOR_WHITE, crs.COLOR_BLACK)
        crs.init_pair(2, 10, crs.COLOR_BLACK)
        crs.init_pair(3, 13, crs.COLOR_BLACK)
        crs.init_pair(4, 15, crs.COLOR_BLACK)
        crs.init_pair(5, crs.COLOR_RED, crs.COLOR_BLACK)
        self.crswin.keypad(True)
        self.graph = Graph()
        if ds_file_name is None:
            crs.echo()
            crswin.addstr(0, 0, "Введите название файла с графами:")
            ds_file_name = crswin.getstr(1, 0)
            crswin.clear()
            crs.noecho()
        self.ds_file_name = ds_file_name
        self.logger = BaseLogger("log.txt","MAIN")
        crswin.addstr(3, 0, "Читаю граф, это будет быстро...")
        self.logger.start_log("read_graph_ds")
        read_graph_ds(self.ds_file_name, self.graph)
        self.logger.end_log()
        self.crswin.clear()
        self.crswin.refresh()
    def clear(self) -> None: self.crswin.clear() 
    def refresh(self) -> None: self.crswin.refresh()

    def endwin(self) -> None: crs.endwin()

    def clear_n_lines(self, start, end) -> None:
        for i in range(start, end+1):
            self.crswin.move(i, 0)
            self.crswin.clrtoeol()
    def wait_for_enter(self, start) -> None:
        crs.curs_set(0)
        self.crswin.attron(crs.color_pair(3))
        self.crswin.addstr(start, 0, "Нажмите ENTER чтобы продолжить")
        self.crswin.attroff(crs.color_pair(3))
        self.crswin.move(start + 1, 0)
        c = self.crswin.getch()
        while c != 10:
            c = self.crswin.getch()
    def validate_number_input(self, input: str) -> int:
        return int(input) if input.isdigit() else -1
    def int_input_handler(self) -> str:
        crs.echo()
        self.crswin.attron(crs.color_pair(4))
        result = self.crswin.getstr()
        self.crswin.attroff(crs.color_pair(4))
        crs.noecho()
        return result
    def string_input_handler(self, row: int) -> str:
        """std::string stringInputHandler(int row)"""
        result = []
        new_typed = []
        i = 0
        c = self.crswin.get_wch()
        while c != '\n':
            # and ((c is int and (c < 258 or c > 261)) or (c is str)):
            if c != -1:
                if c == 127 or c == crs.KEY_BACKSPACE:
                    if i > 0:
                        result.pop()
                        i -= 1
                        self.crswin.move(row, i)
                    else:
                        i = 0

                    self.crswin.clrtoeol()

                elif ord(c) == ctrl(27):
                    return EXIT_APP

                else:
                    self.crswin.attron(crs.color_pair(4))
                    self.crswin.addch(c)
                    self.crswin.attroff(crs.color_pair(4))
                    result.append(c)
                    new_typed.append(c)
                    i = i + 1

            c = self.crswin.get_wch()

        return ''.join(result)
    
    def mode_input_handler(self):
        """int modeInputHandler()"""    
        self.clear()
        self.refresh()
        crs.curs_set(0)
        chosen_mode = 0
        modes_count = 6
        modes = [
            "   1. Среди кратчайших по времени путей между двумя городами найти путь минимальной стоимости",
            " 2. Среди путей между двумя городами найти путь минимальной стоимости",
            " 3. Среди путей между двумя городами найти путь минимальный по числу посещенных городов",
            " 4. Найти множество городов (и минимальных по стоимости путей к ним), достижимых из города отправления  не более чем за обозначенную сумму денег",
            " 5. Найти множество городов (и минимальных по времени путей к ним), достижимых из города отправления не более чем за обозначенное время",  
            "Завершить программу"
            ]
        user_choice = -1
        is_proccessing = True

        #// clear()
        #// refresh()

        self.crswin.addstr(0, 0, "Добро пожаловать! Выберите режим работы")

        while is_proccessing:
          poss = 0
          for i in range(modes_count):
            #for (int i = 0, poss = 0 i < modes_count i++, poss+=2){
            if chosen_mode == i:
              self.crswin.attron(crs.A_REVERSE)
            self.crswin.addstr(poss + TITLE_STEP, 0, modes[i])
            self.crswin.attroff(crs.A_REVERSE)
            poss = poss + 2

          user_choice = self.crswin.getch()
        #обработка клавиш
          if user_choice == crs.KEY_UP:
            chosen_mode = (modes_count - 1) if chosen_mode == 0 else (chosen_mode-1)
          elif user_choice == crs.KEY_DOWN:
            chosen_mode = 0 if (chosen_mode == modes_count - 1) else (chosen_mode+1)
          elif user_choice == 10:
            is_proccessing = False

        res = chosen_mode if chosen_mode != 5 else -1
        log_str = "Был выбран:" + str(res)
        self.logger.log("modeInputHandler", log_str)
        return res
        pass
    def finish_app_handler(self):
        """int finishAppHandler()"""
        crs.curs_set(0)
        modes_count = 2
        chosen_mode = 0
        modes = [
            " 1. Завершить программу",
            " 2. Вернутся на страницу выбора режима работы"
        ]
        user_choice = -1
        is_proccessing = True

        self.crswin.addstr(0, 0, "Выберите режим работы:")
        while is_proccessing:
          for i in range(modes_count):
            if chosen_mode == i:
              self.crswin.attron(crs.A_REVERSE)
            self.crswin.addstr(i + TITLE_STEP, 0, modes[i])
            self.crswin.attroff(crs.A_REVERSE)
          user_choice = self.crswin.getch()
          if user_choice == crs.KEY_UP:
            chosen_mode = (modes_count - 1) if chosen_mode == 0 else (chosen_mode-1)
          elif user_choice == crs.KEY_DOWN:
            chosen_mode = 0 if (chosen_mode == modes_count - 1) else (chosen_mode+1)
          if user_choice == 10:
            return chosen_mode
        return chosen_mode
        pass
    def restricted_weights_handler(self, mess: str) -> int:
        """weight_t RestrictedWeightsHandler(const char *mess)"""
        self.clear()
        self.refresh()

        crs.curs_set(1)
        out_str = "Введите ограничение по"
        self.crswin.addstr(0,0,out_str + mess)
    
        self.crswin.move(1, 0)

        self.refresh()
        validation_input = True
        limit_input = ''
        limit = -1
        while validation_input:
            limit_input = self.int_input_handler()
            try:
                limit = self.validate_number_input(limit_input)
                if limit != -1:
                    validation_input = False
                    continue
                self.clear_n_lines(3, 4)
                self.crswin.addstr(3, 0, "Число введено некорректно, повторите попытку")
                self.crswin.move(4, 0)
            except ValueError:
                pass
        self.clear_n_lines(0, 4)

        self.crswin.attron(crs.color_pair(3))
        self.crswin.addstr(0, 0, "######//~~~~~~//######")
        self.crswin.attroff(crs.color_pair(3))
        out_str = "Введено ограничение: " +  limit_input.decode() + "у. е." 
        if limit == 0:
            self.crswin.attron(crs.color_pair(2))
            self.crswin.addstr(2, 0, "Нет ограничения")
            self.crswin.attroff(crs.color_pair(2))
            limit = MAX_WEIGHT
        else:
            self.crswin.attron(crs.color_pair(2))
            self.crswin.addstr(2, 0, out_str)
            self.crswin.attroff(crs.color_pair(2))
        self.crswin.attron(crs.color_pair(3))
        self.crswin.addstr(4, 0, "######//~~~~~~//######")
        self.crswin.attroff(crs.color_pair(3))

        self.wait_for_enter(6)
        return limit
        pass
    def add_restricted_transport_handler(self) -> int:
        """int addRestrictedTransportHandler( SearchAlgorithm& s)"""
        self.clear()
        self.refresh()

        crs.curs_set(1)
        self.crswin.addstr("Введите запрещенный транспорт или '.'")

        #//this->log("printw()", "getCityHandler")

        self.crswin.move(1, 0)

        #//this->log("move", "getCityHandler")
        tr_name = self.string_input_handler(1)
        validation_input = True   
        while validation_input:
            
            if tr_name == ".":
                validation_input = False
                return 0
                continue  
            
            if not self.sercher.add_restrictions_transport(tr_name): 
                self.clear_n_lines(3, 4)
                self.crswin.addstr(3, 0, "Введите корректный транспорт")
                self.crswin.move(4, 0)
                tr_name = self.string_input_handler(4)
            else:
                validation_input = False  
                #except ValueError:
                #  pass
        self.clear_n_lines(0, 4)
        self.crswin.attron(crs.color_pair(3))
        self.crswin.addstr(0, 0, "######//~~~~~~//######")
        self.crswin.attroff(crs.color_pair(3))
        self.crswin.attron(crs.color_pair(2))
        tr_name = "Ислючен:" + tr_name + "\n" 
        self.crswin.addstr(2, 0, tr_name)
        self.crswin.attroff(crs.color_pair(2))

        self.crswin.attron(crs.color_pair(3))
        self.crswin.addstr(4, 0, "######//~~~~~~//######")
        self.crswin.attroff(crs.color_pair(3))
        self.wait_for_enter(6)
        return 1
        pass
    def get_city_handler(self, to_city = False) -> int:
        """id_t getCityHandler(Graph &graph, bool to_city)"""
        crs.curs_set(1)
        if to_city:
            self.crswin.addstr(0,0, "Введите город прибытия")   
        else:
            self.crswin.addstr(0,0, "Введите город отправления")
        #this->log("printw()", "getCityHandler")

        self.crswin.move(1, 0)

        #//this->log("move", "getCityHandler")
        self.refresh()
    
        city_name = self.string_input_handler(1)
        city_from = -1
        validation_input = True
        while validation_input:
          try:
            self.crswin.addstr(3, 0, city_name)
            city_from = self.graph.find_city_by_name(city_name)
            if city_from != -1:
                validation_input = False
                continue
            self.clear_n_lines(3, 4)
            self.crswin.addstr(3, 0, "Введите корректный город")
            self.crswin.move(4, 0)
            city_name = self.string_input_handler(4)
          except ValueError:
            pass
             

        self.clear_n_lines(0, 4)

        self.crswin.attron(crs.color_pair(3))
        self.crswin.addstr(0, 0, "######//~~~~~~//######")
        self.crswin.attroff(crs.color_pair(3))
        self.crswin.addstr(2, 0, "Ваш город: ")
        self.crswin.attron(crs.color_pair(2))
        self.crswin.addstr(2, 23, city_name)
        self.crswin.attroff(crs.color_pair(2))
        self.crswin.attron(crs.color_pair(3))
        self.crswin.addstr(4, 0, "######//~~~~~~//######")
        self.crswin.attroff(crs.color_pair(3))

        self.wait_for_enter(6)

        return city_from
        pass
    def format_string(self, input: str) -> str:
        """std::string formatString(const std::string &input)"""
        result = input.split('"').join()
        return result
        pass
    def loop(self):
        #self.logger.log("Запуск программы", "loop")
        self.logger.start_log("RUN")
        working = True
        self.sercher = Dijkstra(self.graph, self.logger)
        while working:
          result = self.mode_input_handler()

          self.clear()
          self.refresh()
          if result == -1:
            break
        
          self.logger.start_log("Запрос пользователя")
          from_city = self.get_city_handler()
          self.clear()
          self.refresh()
        
          to_city = ''
          if result < 3:
            to_city = self.get_city_handler(True)
            self.clear()
            self.refresh()

          self.sercher.reset_restrictions_transport()
          while self.add_restricted_transport_handler() != 0:
            pass
          self.logger.log(self.sercher.get_prohibited_transport(), "RestrictedTransportHandler") 
          limits = ''
          
          if result == 3:
            fare_w = self.restricted_weights_handler(" цене\n")
            limits = weights_t (MAX_WEIGHT, fare_w, MAX_WEIGHT)    
          elif result == 4:
            time_w = self.restricted_weights_handler(" времени\n")
            limits = weights_t (time_w, MAX_WEIGHT, MAX_WEIGHT)
          else:
            limits = weights_t (MAX_WEIGHT, MAX_WEIGHT, MAX_WEIGHT)
          weights_log = f"{limits['time']}|{limits['fare']}|{limits['locals']}"
          self.logger.log( weights_log, "ограничения")
          self.sercher.set_limits(limits)
          self.clear()
          self.refresh()
          #self.logger.log("До switch", "loop")
          if result == 0:
              self.logger.start_log("find_fast_cheaper_route")
              try:
                log_title = "Путь минимальной стоимости среди кратчайших по времени"
                city_from_title = f"id:{from_city}|name:{self.graph.find_city_by_id(from_city)}"
                city_to_title = f"id:{to_city}|name:{self.graph.find_city_by_id(to_city)}"
                #self.logger.log(city_from_title, "city_from_title")
                #self.logger.log(city_to_title, "city_to_title")
                if from_city != to_city:
                    path = self.sercher.find_fast_cheaper_route(from_city, to_city)
                    self.logger.end_log()
                    if path != -1:
                        path_str = self.route_to_string(path)
                    else:
                        path_str = "Нет пути"
                    self.crswin.attron(crs.color_pair(4))
                    self.crswin.addstr(0, 0, log_title)
                    self.logger.log(path_str, "find_fast_cheaper_route::")
                    self.crswin.addstr(3, 0, path_str)
                    self.crswin.attroff(crs.color_pair(4))
                else:
                    self.crswin.addstr("Город отправления = городу прибытия")    
              except RuntimeError:
                        self.logger.end_log()
                        self.crswin.addstr("Пути нет")
          elif result == 1:
            self.logger.start_log("find_cheap_route")
            try:
                log_title = "Путь минимальной стоимости"
                #city_from_title = f"id:{from_city}|name:{self.graph.find_city_by_id(from_city)}"
                #city_to_title = f"id:{to_city}|name:{self.graph.find_city_by_id(to_city)}"
                #self.logger.log(city_from_title, "city_from_title")
                #self.logger.log(city_to_title, "city_to_title")
                if from_city != to_city:
                    path = self.sercher.find_cheaper_route(from_city, to_city)
                    self.logger.end_log()
                    if path != -1:
                        path_str = self.route_to_string(path)
                    else:
                        path_str = "Нет пути"
                    self.crswin.attron(crs.color_pair(4))
                    self.crswin.addstr(0, 0, log_title)
                    self.logger.log(path_str, "find_cheaper_route::")
                    self.crswin.addstr(3, 0, path_str)
                    self.crswin.attroff(crs.color_pair(4))
                else:
                    self.crswin.addstr("Город отправления = городу прибытия")        
            except RuntimeError:
                self.logger.end_log()
                self.crswin.addstr("Пути нет")
          elif result == 2:
                self.logger.start_log("find_short_route")
                try:
                    log_title = "Путь кратчайших по времени среди минимальных по стоимости "
                    #city_from_title = f"id:{from_city}|name:{self.graph.find_city_by_id(from_city)}"
                    #city_to_title = f"id:{to_city}|name:{self.graph.find_city_by_id(to_city)}"
                    #self.logger.log(city_from_title, "city_from_title")
                    #self.logger.log(city_to_title, "city_to_title")
                    if from_city != to_city:
                        path = self.sercher.find_short_route(from_city, to_city)
                        self.logger.end_log()
                        if path != -1:
                            path_str = self.route_to_string(path)
                        else:
                            path_str = "Нет пути"
                        self.crswin.attron(crs.color_pair(4))
                        self.crswin.addstr(0, 0, log_title)
                        self.logger.log(path_str, "find_short_route")
                        self.crswin.addstr(3, 0, path_str)
                        self.crswin.attroff(crs.color_pair(4))
                    else:
                        self.crswin.addstr("Город отправления = городу прибытия")            
                except RuntimeError:
                    self.logger.end_log()
                    self.crswin.addstr("Пути нет")
          elif result == 3:
              #получить ограничения 
              self.logger.start_log("find_set_cities_lim_cost")
              try:
                
                #city_from_title = f"id:{from_city}|name:{self.graph.find_city_by_id(from_city)}"
                #city_to_title = f"id:{to_city}|name:{self.graph.find_city_by_id(to_city)}"
                #self.logger.log(city_from_title, "city_from_title")
                #self.logger.log(city_to_title, "city_to_title")
                
                paths = self.sercher.find_set_cities_lim_cost(from_city)
                self.logger.end_log()
                log_title = "Мно-во достижимых за lim_cost"
                path_str_list = []
                if paths != -1:
                    for rt in paths:
                        path_str_list.append(self.route_to_string(rt))
                        path_str_list.append("\n\n")
                    path_str_list = '\n'.join(path_str_list)
                else:
                    path_str_list = "Путей нет"

                self.crswin.attron(crs.color_pair(4))
                self.crswin.addstr(0, 0, log_title)
                self.logger.log(path_str_list, "ind_set_cities_lim_cost")
                self.crswin.addstr(3, 0, path_str_list)
                self.crswin.attroff(crs.color_pair(4))
              except RuntimeError:
                        self.logger.end_log()
                        self.crswin.addstr("Пути нет")
          elif result == 4:
              #получить ограничения 
              self.logger.start_log("find_set_cities_lim_time")
              try:
                
                #city_from_title = f"id:{from_city}|name:{self.graph.find_city_by_id(from_city)}"
                #city_to_title = f"id:{to_city}|name:{self.graph.find_city_by_id(to_city)}"
                #self.logger.log(city_from_title, "city_from_title")
                #self.logger.log(city_to_title, "city_to_title")
                
                paths = self.sercher.find_set_cities_lim_time(from_city)
                self.logger.end_log()
                log_title = "Мно-во достижимых за lim_time"
                path_str_list = []
                if paths != -1:
                    for rt in paths:
                        path_str_list.append(self.route_to_string(rt))
                        path_str_list.append("\n\n")
                    path_str_list = '\n'.join(path_str_list)
                else:
                    path_str_list = "Путей нет"

                self.crswin.attron(crs.color_pair(4))
                self.crswin.addstr(0, 0, log_title)
                self.logger.log(path_str_list, "ind_set_cities_lim_cost")
                self.crswin.addstr(3, 0, path_str_list)
                self.crswin.attroff(crs.color_pair(4))
              except RuntimeError:
                        self.logger.end_log()
                        self.crswin.addstr("Пути нет")
          self.logger.end_log()
          while self.crswin.getch() != 10:
            pass
          # wait_for_enter()

          self.clear()
          self.refresh()

          chosen_mode = self.finish_app_handler()
          if chosen_mode == 0:
            working = False
        self.endwin()
        self.logger.end_log()
        return 0
        pass
    def route_to_string(self, rt):
        from_name = self.graph.find_city_by_id(rt.from_city)
        to_name =  self.graph.find_city_by_id(rt.to_city)
        title = f"{from_name}->{to_name}:{rt.weights}"
        path = []
        path.append(title)
        for fl in rt.path.traverse_forward():
            path.append(self.graph.flight_to_string(fl))
        return '\n'.join(path)   