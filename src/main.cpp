// #include <Arduino.h>
// #include <WiFi.h>
// #include <ArduinoJson.h>
// #include <HTTPClient.h>

// // Prototipo das funcoes
// void conexaoWifi();
// void reconexaoWifi();
// String consultaApi(String cep);

// const char *SSID = "SENAI EDUCACIONAL";
// const char *SENHA = "info@134";

// const unsigned long tempoConexao = 10000;
// const unsigned long tempoReconexao = 5000;

// const String enderecoApi = "https://brasilapi.com.br/api/cep/v2/";

// void setup()
// {
//   Serial.begin(115200);

//   // Conexao WIfi
//   conexaoWifi();

//   String cep = "09330500";

//   String resposta = consultaApi(cep);

//   if (resposta != "")
//   {
//     JsonDocument doc;
//     DeserializationError erro = deserializeJson(doc, resposta);

//     if (!erro)
//     {
//       String rua = doc["street"];
//       String bairro = doc["neighborhood"];
//       String cidade = doc["city"];
//       String estado = doc["state"];

//       Serial.println();
//       Serial.println("DADOS DE ENDERECO");
//       Serial.println("-----------------");
//       Serial.println("CEP: " + cep);
//       Serial.println("Rua: " + rua);
//       Serial.println("Bairro: " + bairro);
//       Serial.println("Cidade: " + cidade);
//       Serial.println("Estado: " + estado);
//     }
//     else
//     {
//       Serial.println("Erro ao interpretar o JSON");
//     }
//   }
// }

// void loop()
// {
//   reconexaoWifi();
// }

// void conexaoWifi()
// {
//   Serial.printf("Conectando ao wifi: %s", SSID);

//   WiFi.begin(SSID, SENHA);

//   unsigned long tempoWifi = millis();

//   while (WiFi.status() != WL_CONNECTED && millis() - tempoWifi < tempoConexao)
//   {
//     Serial.print(".");
//     delay(500);
//   }

//   if (WiFi.status() == WL_CONNECTED)
//   {
//     Serial.println("Wifi conectado com sucesso!");
//     Serial.print("Endereço IP: ");
//     Serial.println(WiFi.localIP());
//   }
//   else
//   {
//     Serial.println("Falha ao se conectar");
//   }
// }

// void reconexaoWifi()
// {
//   unsigned long tempoAtual = millis();
//   unsigned long tempoUltimaConexao = 0;

//   if (tempoAtual - tempoUltimaConexao > tempoReconexao)
//   {
//     if (WiFi.status() != WL_CONNECTED)
//     {
//       Serial.println("Conexao Wifi perdida");
//       conexaoWifi();
//     }
//     tempoUltimaConexao = tempoAtual;
//   }
// }
// // ------------------------------------------------------

// String consultaApi(String cep)
// {
//   HTTPClient http;

//   http.begin(enderecoApi + cep);

//   int codigoHttp = http.GET();
//   String resposta = "";

//   delay(3000);

//   if (codigoHttp == 200)
//   {
//     resposta = http.getString();
//   }
//   else
//   {
//     Serial.println("Erro na requisicao HTTP: " + String(codigoHttp));
//   }

//   http.end();

//   return resposta;
// }


#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "SENAI EDUCACIONAL";
const char* password = "info@134";

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
}

void loop() {

  Serial.println("\nDigite o CEP:");

  while (Serial.available() == 0) {
    delay(100);
  }

  String cep = Serial.readStringUntil('\n');
  cep.trim();

  if (cep.length() == 0) {
    Serial.println("Digite um CEP!");
    return;
  }

  if (cep.length() != 8) {
    Serial.println("CEP invalido! Digite 8 numeros.");
    return;
  }

  for (int i = 0; i < cep.length(); i++) {
    if (!isDigit(cep[i])) {
      Serial.println("CEP invalido! Digite apenas numeros.");
      return;
    }
  }

  HTTPClient http;

  String url = "https://brasilapi.com.br/api/cep/v1/" + cep;

  http.begin(url);

  int httpCode = http.GET();

  if (httpCode == 200) {

    String resposta = http.getString();

    JsonDocument doc;
    DeserializationError erro = deserializeJson(doc, resposta);

    if (!erro) {
      Serial.println("\nCEP encontrado!");
      Serial.println("CEP: " + doc["cep"].as<String>());
      Serial.println("Estado: " + doc["state"].as<String>());
      Serial.println("Cidade: " + doc["city"].as<String>());
      Serial.println("Bairro: " + doc["neighborhood"].as<String>());
      Serial.println("Rua: " + doc["street"].as<String>());
    }

  } else if (httpCode == 404) {

    Serial.println("CEP nao encontrado!");

  } else {

    Serial.print("Erro na consulta: ");
    Serial.println(httpCode);

  }

  http.end();

  delay(1000);
}
