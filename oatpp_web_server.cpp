#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include <vector>
#include <mutex>
#include <fstream>

class LogService {
private:
    std::vector<std::string> logs;
    std::mutex logMutex;
public:
    void addLog(const std::string& log) {
        std::lock_guard<std::mutex> lock(logMutex);
        logs.push_back(log);
    }
    std::string getLogs() {
        std::lock_guard<std::mutex> lock(logMutex);
        std::string logStr;
        for (const auto& log : logs) {
            logStr += log + "\n";
        }
        return logStr.empty() ? "Лог пуст" : logStr;
    }
};

class MyController : public oatpp::web::server::api::ApiController {
private:
    std::shared_ptr<LogService> logService;
public:
    MyController(const std::shared_ptr<ObjectMapper>& objectMapper, std::shared_ptr<LogService> logService)
        : oatpp::web::server::api::ApiController(objectMapper), logService(logService) {}

    ENDPOINT("GET", "/info", info) {
        logService->addLog("[INFO] Запрос к /info");
        return createResponse(Status::CODE_200, "Всё ок");
    }
    
    ENDPOINT("GET", "/log", getLog) {
        return createResponse(Status::CODE_200, logService->getLogs());
    }
    
    ENDPOINT("POST", "/upload", uploadFile, 
             BODY_STRING(String, body),
             HEADER(String, contentType, "Content-Type")) {
        logService->addLog("[UPLOAD] Получен файл");
        
        std::ofstream file("/tmp/uploaded_file", std::ios::binary);
        if (!file) {
            return createResponse(Status::CODE_500, "Ошибка сохранения файла");
        }
        file << body;
        file.close();
        
        return createResponse(Status::CODE_200, "Файл загружен в /tmp/");
    }
};

void run() {
    oatpp::base::Environment::init();
    auto logService = std::make_shared<LogService>();
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto router = oatpp::web::server::HttpRouter::createShared();
    auto controller = std::make_shared<MyController>(objectMapper, logService);
    controller->addEndpointsToRouter(router);
    
    auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 1616, oatpp::network::Address::IPV4});
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    
    logService->addLog("Сервер запущен на порту 1616");
    server.run();
    oatpp::base::Environment::destroy();
}

int main() {
    run();
    return 0;
}