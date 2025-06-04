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

#### Observação

Caso esteja rodando a API no Windows com WSL instalado você vai precisar adicionar uma regra no firewall do Windows pois por padrão o firewall bloqueia esse tipo de requisição.

##### Abra Windows defender firewall com Advanced Security:

* Pressione Win + R, digite wf.msc, e aperte Enter.
* Crie uma inbound rule:
* Escolha `Port`, e clique `Next`.
* Selecione `TCP and Specific local ports`, e digite 8000. Clique `Next`.
* Selecione `Allow the connection`, clique `Next`.
* Selecione o profile mais adequado (Domain, Private, Public). Selecione `Private`. Clique `Next`.

Dê um nome a regra (e.g., “WSL2 FastAPI Port 8000”). Clique `Finish`

##### Port forward para o IP da instância WSL

1. Descubra o IP da sua instância WSL:
  - No terminal WSL digite `hostname -I`
2. Execute o Powershell no mode administrador
3. Cole o seguinte comando:

```powershell
netsh interface portproxy add v4tov4 `
  listenport=8000 `
  listenaddress=0.0.0.0 `
  connectport=8000 `
  connectaddress=<IP-RETORNADO-NO-PASSO-1>
``` 

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
