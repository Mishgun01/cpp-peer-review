struct DBInfo {
    string_view db_name;
    int db_connection_timeout = 0;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};

struct PersonInfo {
    int min_age = 0;
    int max_age = 0;
    string_view name_filter;
};

// Функция для подключения к базе данных с возможностью выбора имени базы данных
DBHandler ConnectToDatabase(const DBInfo& db_info, const string_view& default_db_name) {
    DBConnector connector(db_info.db_allow_exceptions, db_info.db_log_level);
    // Если имя базы данных начинается с "tmp.", используется имя из DBInfo, в противном случае - имя по умолчанию
    const string_view& db_name = db_info.db_name.starts_with("tmp."s) ? db_info.db_name : default_db_name;
    return connector.Connect(db_name, db_info.db_connection_timeout);
}

vector<Person> LoadPersons(const DBInfo& db_info, const PersonInfo& person_info) {
    const string_view& default_db_name = "default_db_name"s;
    // Подключаемся к базе данных
    DBHandler db = ConnectToDatabase(db_info, default_db_name);

    if (!db.IsOK()) {
        // Если подключение не удалось и исключения не разрешены, возвращаем пустой вектор
        if (!db_info.db_allow_exceptions) {
            return {};
        }

        // Строим SQL-запрос для выборки персон с учетом фильтров
        ostringstream query_str;
        query_str << "FROM Persons "s
            << "SELECT Name, Age "s
            << "WHERE Age BETWEEN "s << person_info.min_age << " AND "s << person_info.max_age << " "s
            << "AND Name LIKE '%"s << db.Quote(person_info.name_filter) << "%'"s;

        DBQuery query(query_str.str());

        // Инициализируем вектор для хранения персон
        vector<Person> persons;
        // Загружаем результаты запроса и добавляем их в вектор
        for (auto [name, age] : db.LoadRows<string, int>(query)) {
            persons.push_back({move(name), age});
        }
        // Возвращаем вектор с данными
        return persons;
    }

    // Если подключение удалось, но база данных не содержит данных, возвращаем пустой вектор
    return {};
}
