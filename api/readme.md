## API de Previsão de Enchentes

Esta API, construída com FastAPI, tem como objetivo prever a ocorrência de enchentes com base nos dados de precipitação dos últimos 7 dias.

### Funcionalidade

A API expõe um endpoint principal (`/predict`) que recebe uma sequência de dados de precipitação e utiliza um modelo de Machine Learning pré-treinado para determinar se há um risco de enchente.

### Endpoint: `/predict`

*   **Método:** `POST`
*   **Descrição:** Recebe os dados de precipitação e retorna uma previsão sobre a ocorrência de enchente.
*   **Entrada (`Request Body`):**
    *   Uma lista de 7 números (`List[float]`). Cada número na lista representa a precipitação total (em milímetros) para um dia, começando pelo dia mais recente (índice 0) e indo até 7 dias atrás (índice 6).
    *   Exemplo: `[10.5, 5.0, 0.0, 2.1, 15.8, 3.0, 0.0]`
*   **Saída (`Response`):**
    *   Um objeto JSON com a chave `"enchente"` (`bool`).
    *   `{"enchente": true}`: Indica que uma enchente é prevista.
    *   `{"enchente": false}`: Indica que uma enchente não é prevista.

### Como Rodar a API

1.  **Pré-requisitos:** Certifique-se de ter Python instalado. É altamente recomendado usar um ambiente virtual.
2.  **Instalar Dependências:** Intale as dependencia do requirements.txt presente na raiz do projeto:
    ```bash
    pip install -r requirements.txt
    ```
3.  **Obter o Modelo:** Certifique-se de que os arquivos do modelo (`knn.joblib` e `scaler.joblib`) estejam no caminho relativo correto (`../modelo/`).
4.  **Executar a API:** No diretório `api`, execute o seguinte comando:
    ```bash
    uvicorn main:app --host 0.0.0.0 --port 8000
    ```
    A API estará disponível em `http://localhost:8000`.

### Sobre o Modelo de Machine Learning

A API utiliza um modelo de classificação (KNN - K-Nearest Neighbors) treinado para prever enchentes. O modelo foi alimentado com características derivadas da precipitação dos últimos 7 dias, incluindo:

*   Precipitação do último dia (`Precip_1d`)
*   Precipitação acumulada nos últimos 3 dias (`Precip_3d`)
*   Precipitação acumulada nos últimos 7 dias (`Precip_7d`)
*   A razão entre a precipitação do último dia e a dos últimos 7 dias (`Ratio_1d_7d`)
*   A "aceleração" da chuva (diferença entre a chuva do dia e a média dos últimos 3 dias) (`Acceleration`)
*   Um "score de impacto" composto que dá mais peso à chuva recente (`Impact_Score`)

Para uma explicação detalhada sobre a criação dessas características, o treinamento do modelo e a comparação com outros algoritmos, consulte o notebook [clicando aqui](../modelo/classificacao_algoritmos.ipynb).

### Testando a API

Você pode testar a API usando ferramentas como `curl`, Postman, [ESP32](../esp32/diagram.json) ou a interface interativa do FastAPI (disponível em `/docs` quando a API está rodando).

Exemplo usando `curl`:

```bash
curl -Method POST "http://localhost:8000/predict" -H @{'accept' = 'application/json'; 'Content-Type' = 'application/json'} -Body "[10.0, 11, 25, 30, 0, 10, 5]"
```