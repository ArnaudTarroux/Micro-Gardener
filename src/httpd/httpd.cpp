#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <esp_http_server.h>

#include "./httpd.h"

static esp_err_t not_found_handler(httpd_req_t *req) {
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type,Access-Control-Allow-Origin");
  httpd_resp_set_hdr(req, "Content-Type", "application/json");
  httpd_resp_send_404(req);
  return ESP_OK;
}

httpd_uri_t uri_not_found = {
  .uri      = "/*",
  .method   = HTTP_GET,
  .handler  = not_found_handler,
  .user_ctx = NULL
};

static esp_err_t health_handler(httpd_req_t *req) {
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type,Access-Control-Allow-Origin");
  httpd_resp_set_hdr(req, "Content-Type", "application/json");
  httpd_resp_sendstr(req, "{\"health\": \"OK\"}");
  return ESP_OK;
}

httpd_uri_t uri_get_health = {
  .uri      = "/health",
  .method   = HTTP_GET,
  .handler  = health_handler,
  .user_ctx = NULL
};

static esp_err_t option_handler(httpd_req_t *req) {
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type,Access-Control-Allow-Origin");
  httpd_resp_sendstr(req, "OK");
  return ESP_OK;
}

httpd_uri_t uri_option = {
  .uri      = "/*",
  .method   = HTTP_OPTIONS,
  .handler  = option_handler,
  .user_ctx = NULL
};

static httpd_handle_t server = NULL;



void httpd_init() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.uri_match_fn = httpd_uri_match_wildcard;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_option);
        httpd_register_uri_handler(server, &uri_get_health);
        httpd_register_uri_handler(server, &uri_not_found);

        return;
    }

    ESP_LOGI(HTTPD_LOG_TAG, "Error starting server.");
}