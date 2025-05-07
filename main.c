#include "mongoose/mongoose.h"
#include "input/input.h"
#include "constants/constants.h"
#include <stdlib.h>
#include <string.h>

// Цены на товары (можно вынести в constants.h)
#define PRICE_BREAD   50
#define PRICE_MILK    80
#define PRICE_CHEESE  120

// Коды ошибок
enum {
    ERR_OK = 0,
    ERR_MISSING_DATA = 1,
    ERR_FILE_NOT_FOUND = 2
};

// Функция обработки запросов
static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
    const char *ctype = "";
    char *response = NULL;
    int status_code = 200;
    int error_code = ERR_OK;

    // Обработка POST /calculate
    if (mg_strcmp(hm->uri, mg_str("/calculate")) == 0 && 
        mg_strcmp(hm->method, mg_str("POST")) == 0) {

        // Извлечение данных из формы
        char bread[10], milk[10], cheese[10];
        char bread_qty[10], milk_qty[10], cheese_qty[10];
        int total = 0;

        mg_http_get_var(&hm->body, "bread", bread, sizeof(bread));
        mg_http_get_var(&hm->body, "milk", milk, sizeof(milk));
        mg_http_get_var(&hm->body, "cheese", cheese, sizeof(cheese));
        mg_http_get_var(&hm->body, "bread_qty", bread_qty, sizeof(bread_qty));
        mg_http_get_var(&hm->body, "milk_qty", milk_qty, sizeof(milk_qty));
        mg_http_get_var(&hm->body, "cheese_qty", cheese_qty, sizeof(cheese_qty));

        // Расчет стоимости
        if (strcmp(bread, "1") == 0) total += atoi(bread_qty) * PRICE_BREAD;
        if (strcmp(milk, "1") == 0) total += atoi(milk_qty) * PRICE_MILK;
        if (strcmp(cheese, "1") == 0) total += atoi(cheese_qty) * PRICE_CHEESE;

        // Загрузка шаблона результата
        response = read_file(PATH_ORDER_RESULT_HTML);
        if (response) {
            // Замена {{TOTAL}} на сумму
            char *placeholder = strstr(response, "{{TOTAL}}");
            if (placeholder) {
                snprintf(placeholder, 15, "%d", total); // 15 - длина буфера
                ctype = CONTENT_TYPE_HTML;
            } else {
                error_code = ERR_MISSING_DATA;
            }
        } else {
            error_code = ERR_FILE_NOT_FOUND;
        }

    } else if (mg_strcmp(hm->uri, mg_str("/styles.css")) == 0) { 
        // Отдача CSS
        response = read_file(PATH_CSS_STYLES);
        ctype = CONTENT_TYPE_CSS;

    } else { 
        // Главная страница с формой заказа
        response = read_file(PATH_ORDER_FORM_HTML);
        ctype = CONTENT_TYPE_HTML;
    }

    // Отправка ответа
    if (response && error_code == ERR_OK) {
        mg_http_reply(c, status_code, ctype, "%s", response);
    } else {
        mg_http_reply(c, 500, "", "Internal Server Error");
    }

    free(response); // Освобождение памяти
    return error_code;
}

// Обработчик событий
static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        process_request(c, hm);
    }
}

int main() {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://localhost:8081", fn, NULL);
    printf("Сервер запущен на http://localhost:8081\n");

    while (true) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    return 0;
}