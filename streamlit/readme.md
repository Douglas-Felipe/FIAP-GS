# Previsão de Enchente com Base na Precipitação dos Últimos 7 Dias

Esta é uma aplicação Streamlit desenvolvida para prever o risco de enchente com base nos dados de precipitação dos últimos 7 dias. A ferramenta utiliza um modelo de Machine Learning (K-Nearest Neighbors - KNN) treinado previamente para classificar o risco como "Alto" ou "Normal".

## Funcionalidades

*   Interface amigável para inserção dos dados de precipitação diária (mm).
*   Cálculo automático de features relevantes a partir dos dados de entrada.
*   Exibição dos dados calculados para transparência do processo.
*   Previsão do risco de enchente utilizando um modelo KNN e um scaler pré-treinados.
*   Apresentação clara do resultado da previsão: "Alto risco de enchente" ou "Sem risco de enchente".
*   Explicação detalhada das features calculadas diretamente na interface.

## Como Funciona

A aplicação opera em alguns passos principais:

1.  **Carregamento do Modelo e Scaler:**
    *   Verifica a existência dos arquivos `knn.joblib` (modelo KNN) e `scaler.joblib` (scaler) na pasta `../modelo/`.
    *   Se não encontrados, exibe um erro e interrompe a execução.
    *   Caso contrário, carrega o modelo e o scaler.

2.  **Interface do Usuário:**
    *   O usuário insere os valores de precipitação (em mm) para o dia atual e os 6 dias anteriores através de campos numéricos.

3.  **Cálculo de Features (A "Fórmula"):**
    *   Com base nos 7 valores de precipitação inseridos, as seguintes features são calculadas:
        *   `Precip_1d`: Precipitação no dia atual.
        *   `Precip_3d`: Soma da precipitação nos últimos 3 dias (incluindo o dia atual).
        *   `Precip_7d`: Soma da precipitação nos últimos 7 dias (incluindo o dia atual).
        *   `Ratio_1d_7d`: Proporção entre a precipitação do dia atual e o total dos últimos 7 dias (adiciona-se um `epsilon` pequeno ao denominador para evitar divisão por zero).
        *   `Acceleration`: Diferença entre a precipitação do dia atual e a média de precipitação dos últimos 3 dias.
        *   `Impact_Score`: Um índice ponderado que combina a precipitação do dia atual ao quadrado, a precipitação dos últimos 3 dias elevada a 1.5, e a precipitação total dos 7 dias.
    *   Estas features são exibidas em um DataFrame para o usuário.
    *   Uma seção expansível ("Clique aqui para ver a explicação dos dados calculados") detalha o nome amigável e a descrição de cada feature.

4.  **Previsão:**
    *   As features calculadas são transformadas (escalonadas) usando o `scaler` carregado.
    *   O modelo KNN (`modelo`) utiliza as features escalonadas para fazer a previsão (0 para sem risco, 1 para alto risco).

5.  **Resultado:**
    *   Se a previsão for 1, um alerta de "Alto risco de enchente" é exibido.
    *   Se a previsão for 0, uma mensagem de "Sem risco de enchente" é mostrada.

## Estrutura do Projeto

Para que a aplicação `app.py` funcione corretamente, os arquivos do modelo e do scaler devem estar localizados em uma pasta chamada `modelo`, que deve ser um diretório irmão da pasta `streamlit` (onde `app.py` reside).

```
GS/
├── modelo/
│   ├── knn.joblib
│   └── scaler.joblib
└── streamlit/
    └── app.py
    └── readme.md  <-- Este arquivo
```

## Pré-requisitos

Certifique-se de ter Python instalado (versão 3.7 ou superior recomendada). Você precisará das seguintes bibliotecas Python, que podem ser instaladas via pip:

```bash
pip install streamlit pandas joblib scikit-learn
```

## Como Rodar a Aplicação

1.  **Clone o repositório ou baixe os arquivos** para sua máquina local.
2.  **Verifique a estrutura de pastas:** Certifique-se de que os arquivos `knn.joblib` e `scaler.joblib` estão presentes na pasta `../modelo/` em relação ao local do `app.py` (conforme a estrutura descrita acima).
3.  **Navegue até o diretório da aplicação:** Abra o terminal ou prompt de comando e navegue até a pasta onde o arquivo `app.py` está localizado (ex: `\FIAP-GS\streamlit\`).
    ```bash
    cd \FIAP-GS\streamlit\
    ```
4.  **Execute o script Streamlit:**
    ```bash
    streamlit run app.py
    ```
5.  A aplicação será aberta automaticamente no seu navegador web padrão. Insira os dados de precipitação para obter a previsão.

## Detalhes das Features Calculadas (A "Fórmula" Explicada)

A precisão da previsão de enchente depende crucialmente das features engenheiradas a partir dos dados brutos de precipitação. Abaixo, detalhamos cada uma delas conforme implementado no script:

*   **`Precip_1d` (Precipitação no Dia atual (mm))**
    *   **Cálculo:** `precip_series.iloc[0]` (Primeiro valor da série de precipitações, correspondente ao dia atual)
    *   **Descrição:** Total de precipitação registrado apenas no dia mais recente. É um indicador direto da chuva mais imediata.

*   **`Precip_3d` (Precipitação nos Últimos 3 Dias (mm))**
    *   **Cálculo:** `precip_series.iloc[0:3].sum()` (Soma dos três primeiros valores da série: dia atual, 1 dia antes, 2 dias antes)
    *   **Descrição:** Soma da precipitação acumulada nos últimos 3 dias. Ajuda a entender a saturação recente do solo e o volume de água que pode estar contribuindo para rios e córregos.

*   **`Precip_7d` (Precipitação nos Últimos 7 Dias (mm))**
    *   **Cálculo:** `precip_series.sum()` (Soma de todos os 7 valores de precipitação inseridos)
    *   **Descrição:** Soma da precipitação total acumulada ao longo da última semana. Fornece uma visão do volume total de água que o sistema hídrico recebeu recentemente.

*   **`Ratio_1d_7d` (Proporção Dia atual / 7 Dias)**
    *   **Cálculo:** `precip_1d / (precip_7d + epsilon)` (onde `epsilon = 1e-6` para evitar divisão por zero caso `precip_7d` seja 0)
    *   **Descrição:** Representa a fração da chuva total da semana que ocorreu no dia atual. Um valor alto pode indicar um evento de chuva súbita e intensa em comparação com a média da semana, aumentando o risco de escoamento superficial rápido.

*   **`Acceleration` (Aceleração da Precipitação)**
    *   **Cálculo:** `precip_1d - (precip_3d / 3)`
    *   **Descrição:** Mede a diferença entre a precipitação do dia atual e a média de precipitação dos últimos 3 dias.
        *   Um valor positivo sugere um aumento na intensidade da chuva em relação à média recente.
        *   Um valor negativo indica uma desaceleração ou diminuição da chuva.
        *   Essa feature ajuda a identificar tendências de intensificação da precipitação.

*   **`Impact_Score` (Índice de Impacto)**
    *   **Cálculo:** `(precip_1d ** 2) + (precip_3d ** 1.5) + precip_7d`
    *   **Descrição:** Um índice composto projetado para capturar o impacto potencial da chuva, dando pesos diferentes para a temporalidade e intensidade da precipitação.
        *   `precip_1d ** 2`: A precipitação do dia atual é elevada ao quadrado, amplificando o efeito de chuvas muito recentes e intensas.
        *   `precip_3d ** 1.5`: A precipitação acumulada nos últimos 3 dias é elevada a 1.5, dando um peso significativo, mas menor que a do dia atual.
        *   `precip_7d`: A precipitação total dos últimos 7 dias é adicionada linearmente.
    *   Este score visa priorizar cenários onde chuvas fortes e concentradas ocorreram recentemente.

Estas features combinadas fornecem ao modelo KNN uma visão multifacetada da situação pluviométrica, permitindo uma previsão mais robusta do risco de enchente.