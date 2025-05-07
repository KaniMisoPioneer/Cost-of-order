#include "mongoose/mongoose.h"
#include "input/input.h"
#include "constants/constants.h"
#include <stdlib.h>
#include <string.h>

enum {
    ERR_OK = 0,
    ERR_MISSING_DATA = 1,
    ERR_FILE_NOT_FOUND = 2
};

static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
    const char *ctype = "";
    char *response = NULL;
    int status_code = 200;
    int error_code = ERR_OK;

    if (mg_strcmp(hm->uri, mg_str("/calculate")) == 0 && 
        mg_strcmp(hm->method, mg_str("POST")) == 0) {

        char bread[10], flour[10], milk[10], cheese[10], sausage[10],
             fish[10], apple[10], banana[10], potato[10], carrot[10];
        char bread_qty[10], flour_qty[10], milk_qty[10], cheese_qty[10], sausage_qty[10],
             fish_qty[10], apple_qty[10], banana_qty[10], potato_qty[10], carrot_qty[10];
        int total = 0;

        mg_http_get_var(&hm->body, "bread", bread, sizeof(bread));
        mg_http_get_var(&hm->body, "flour", flour, sizeof(flour));
        mg_http_get_var(&hm->body, "milk", milk, sizeof(milk));
        mg_http_get_var(&hm->body, "cheese", cheese, sizeof(cheese));
        mg_http_get_var(&hm->body, "sausage", sausage, sizeof(sausage));
        mg_http_get_var(&hm->body, "fish", fish, sizeof(fish));
        mg_http_get_var(&hm->body, "apple", apple, sizeof(apple));
        mg_http_get_var(&hm->body, "banana", banana, sizeof(banana));
        mg_http_get_var(&hm->body, "potato", potato, sizeof(potato));
        mg_http_get_var(&hm->body, "carrot", carrot, sizeof(carrot));

        mg_http_get_var(&hm->body, "bread_qty", bread_qty, sizeof(bread_qty));
        mg_http_get_var(&hm->body, "flour_qty", flour_qty, sizeof(flour_qty));
        mg_http_get_var(&hm->body, "milk_qty", milk_qty, sizeof(milk_qty));
        mg_http_get_var(&hm->body, "cheese_qty", cheese_qty, sizeof(cheese_qty));
        mg_http_get_var(&hm->body, "sausage_qty", sausage_qty, sizeof(sausage_qty));
        mg_http_get_var(&hm->body, "fish_qty", fish_qty, sizeof(fish_qty));
        mg_http_get_var(&hm->body, "apple_qty", apple_qty, sizeof(apple_qty));
        mg_http_get_var(&hm->body, "banana_qty", banana_qty, sizeof(banana_qty));
        mg_http_get_var(&hm->body, "potato_qty", potato_qty, sizeof(potato_qty));
        mg_http_get_var(&hm->body, "carrot_qty", carrot_qty, sizeof(carrot_qty));

        if (strcmp(bread, "1") == 0) total += atoi(bread_qty) * PRICE_BREAD;
        if (strcmp(flour, "1") == 0) total += atoi(flour_qty) * PRICE_FLOUR;
        if (strcmp(milk, "1") == 0) total += atoi(milk_qty) * PRICE_MILK;
        if (strcmp(cheese, "1") == 0) total += atoi(cheese_qty) * PRICE_CHEESE;
        if (strcmp(sausage, "1") == 0) total += atoi(sausage) * PRICE_SAUSAGE;
        if (strcmp(fish, "1") == 0) total += atoi(fish_qty) * PRICE_FISH;
        if (strcmp(apple, "1") == 0) total += atoi(apple_qty) * PRICE_APPLE;
        if (strcmp(banana, "1") == 0) total += atoi(banana_qty) * PRICE_BANANA;
        if (strcmp(potato, "1") == 0) total += atoi(potato_qty) * PRICE_POTATO;
        if (strcmp(carrot, "1") == 0) total += atoi(carrot_qty) * PRICE_CARROT;

        response = read_file(PATH_ORDER_RESULT_HTML);
        if (response) {
            char *placeholder = strstr(response, "{{TOTAL}}");
            if (placeholder) {
                snprintf(placeholder, 15, "%d", total);
                ctype = CONTENT_TYPE_HTML;
            } else {
                error_code = ERR_MISSING_DATA;
            }
        } else {
            error_code = ERR_FILE_NOT_FOUND;
        }

    } else if (mg_strcmp(hm->uri, mg_str("/css/styles.css")) == 0) { 
        response = read_file(PATH_CSS_STYLES);
        if (response) {
            mg_http_reply(c, 200, CONTENT_TYPE_CSS, "%s", response);
            error_code = ERR_OK;
        } else {
            mg_http_reply(c, 404, CONTENT_TYPE_HTML, "CSS not found");
            error_code = ERR_FILE_NOT_FOUND;
        }
    } else { 
        response = read_file(PATH_ORDER_FORM_HTML);
        ctype = CONTENT_TYPE_HTML;
    }

    if (response && error_code == ERR_OK) {
        mg_http_reply(c, status_code, ctype, "%s", response);
    } else {
        mg_http_reply(c, 500, "", "Internal Server Error");
    }

    free(response);
    return error_code;
}

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