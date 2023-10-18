// Структура для запроса на парсинг JSON-объекта с информацией о городах
struct ParseCitySubjsonRequest {
  const string& country_name;
  const string& country_iso_code;
  const string& country_phone_code;
  const string& country_time_zone;
  const vector<Language>& languages;
};

// Функция для парсинга JSON-объекта с городами конкретной страны
void ParseCitySubjson(vector<City>& cities, const Json& json, const string& country_name,
                      const string& country_iso_code, const string& country_phone_code, const string& country_time_zone,
                      const vector<Language>& languages) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        // Создаем город и добавляем его в список городов
        cities.push_back({
            city_obj["name"s].AsString(),
            city_obj["iso_code"s].AsString(),
            country_phone_code + city_obj["phone_code"s].AsString(),
            country_name,
            country_iso_code,
            country_time_zone,
            languages
        });
    }
}

// Функция ParseCitySubjson перегружена для использования структуры ParseCitySubjsonRequest
void ParseCitySubjson(vector<City>& cities, const Json& json, ParseCitySubjsonRequest parse_city_subject_request) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        // Создаем город, используя информацию из запроса, и добавляем его в список городов
        cities.push_back({
            city_obj["name"s].AsString(),
            city_obj["iso_code"s].AsString(),
            parse_city_subject_request.country_phone_code + city_obj["phone_code"s].AsString(),
            parse_city_subject_request.country_name,
            parse_city_subject_request.country_iso_code,
            parse_city_subject_request.country_time_zone,
            parse_city_subject_request.languages
        });
    }
}

// Функция для парсинга JSON-объекта с информацией о стране
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        
        // Создаем и добавляем страну в список стран
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString()
        });
        
        Country& country = countries.back();
        
        // Парсим языки страны и добавляем их к стране
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        
        // Вызываем парсинг информации о городах, передавая данные о стране
        ParseCitySubjson(cities, country_obj["cities"s], country.name, country.iso_code, country.phone_code,
                         country.time_zone, country.languages);
        
        // Вызываем парсинг информации о городах, используя структуру запроса
        ParseCitySubjson(cities, country_obj["cities"s], {
            .country_name = country.name,
            .country_iso_code = country.iso_code,
            .country_phone_code = country.phone_code,
            .country_time_zone = country.time_zone,
            .languages = country.languages
        });
    }
}
