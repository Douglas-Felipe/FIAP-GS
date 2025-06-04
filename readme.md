
# FIAP - Faculdade de Informática e Administração Paulista 

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="assets/logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>


## Grupo DRELL

## 👨‍🎓 Integrantes: 
- <a href="https://www.linkedin.com/in/douglas-souza-felipe-b815281a2/">Douglas</a>
- <a href="https://www.linkedin.com/in/richard-marques-26b3a14/">Richard</a>
- <a href="https://www.linkedin.com/in/lucasmedeirosleite">Lucas Medeiros</a> 
- <a href="https://www.linkedin.com/in/evelyn-z-342a07365/">Evelyn Z</a> 
- <a href="https://www.linkedin.com/in/luis-fernando-dos-santos-costa-b69894365/">Luis</a>

## 👩‍🏫 Professores:
### Tutor(a) 
- <a href="https://github.com/leoruiz197">Leo Ruiz</a>
### Coordenador(a)
- <a href="https://www.linkedin.com/in/andregodoichiovato/">Andre Godoi</a>

# Sistema de Previsão de Enchentes

Este projeto tem como objetivo desenvolver um sistema capaz de prever a ocorrência de enchentes com base nos dados de precipitação dos últimos 7 dias, utilizando um modelo de Machine Learning. A intenção é fornecer uma ferramenta que possa auxiliar em sistemas de alerta precoce e na tomada de decisões para mitigação de danos.

## Problema Proposto

Enchentes são desastres naturais que causam grandes prejuízos materiais e humanos. A capacidade de prever esses eventos com antecedência é crucial para minimizar seus impactos. Este projeto busca resolver esse problema analisando dados históricos de precipitação e ocorrência de enchentes para treinar um modelo preditivo.

## Visão Geral do Projeto

O sistema é composto por diferentes módulos que trabalham em conjunto:

1.  **`esp32/` - Coleta de Dados (Hardware)**:
    *   Responsável pela coleta de dados de precipitação em tempo real (ou simulado) utilizando um microcontrolador ESP32.
    *   Para mais detalhes sobre a montagem e o código do ESP32, consulte o [README do ESP32](./esp32/readme.md).

2.  **`modelo/` - Modelagem e Treinamento de Machine Learning**:
    *   Nesta etapa, os dados de precipitação são processados e utilizados para treinar um modelo de classificação.
    *   Foram exploradas diversas características (features) derivadas da precipitação dos últimos 7 dias, como `Precip_1d`, `Precip_3d`, `Precip_7d`, `Ratio_1d_7d`, `Acceleration` e `Impact_Score`.
    *   O modelo escolhido foi o KNN (K-Nearest Neighbors), que, juntamente com um normalizador (scaler), é salvo (`knn.joblib` e `scaler.joblib`) para ser utilizado pela API de previsão e pelo dashboard Streamlit.
    *   Para uma análise detalhada da engenharia de features, treinamento do modelo e comparação com outros algoritmos, veja o [notebook classificacao_algoritmos.ipynb](./modelo/classificacao_algoritmos.ipynb).

3.  **`api/` - API de Previsão**:
    *   Uma API construída com FastAPI que expõe um endpoint (`/predict`).
    *   Este endpoint recebe uma lista com os dados de precipitação dos últimos 7 dias e utiliza o modelo KNN treinado para retornar uma previsão booleana indicando se há risco de enchente (`{"enchente": true/false}`).
    *   Para instruções sobre como rodar, testar e interagir com a API, consulte o [README da API](./api/readme.md).

4.  **`streamlit/` - Dashboard de Visualização**:
    *   Uma aplicação web interativa desenvolvida com Streamlit para visualizar os dados e as previsões do modelo.
    *   Para mais informações sobre como executar e utilizar o dashboard, veja o [README do Streamlit](./streamlit/readme.md).

5. **`datasets/` - Dados e como foram coletados**:
    *   Dados coletados e salvos em um arquivo `.csv` junto com uma explicação do processo de coleta, tratamento e modelagem dos dados, veja [README do Dataset](./datasets/readme.md)


## Configuração e Instalação do Ambiente

Siga os passos abaixo para configurar o ambiente de desenvolvimento e rodar o projeto.

1.  **Pré-requisitos**:
    *   Python 3.8 ou superior instalado.
    *   Git (opcional, para clonar o repositório).

2.  **Clonar o Repositório (Opcional)**:
    Se você ainda não tem o projeto, clone o repositório:
    ```bash
    git clone git@github.com:Douglas-Felipe/FIAP-GS.git
    cd FIAP-GS
    ```

3.  **Criar um Ambiente Virtual (Recomendado)**:
    É uma boa prática usar ambientes virtuais para isolar as dependências do projeto.
    ```bash
    python -m venv venv
    ```
    Ative o ambiente virtual:
    *   **Windows**:
        ```bash
        .\venv\Scripts\activate
        ```
    *   **macOS/Linux**:
        ```bash
        source venv/bin/activate
        ```

4.  **Instalar Dependências**:
    Todas as bibliotecas Python necessárias estão listadas no arquivo `requirements.txt` na raiz do projeto. Para instalá-las, execute:
    ```bash
    pip install -r requirements.txt
    ```

## Executando o Projeto

Cada componente do projeto (API, Dashboard Streamlit) possui suas próprias instruções para execução. Consulte os respectivos arquivos README para obter orientações detalhadas:

*   **API de Previsão**: Veja as instruções em [api/README.md](./api/readme.md).
*   **Dashboard Streamlit**: Veja as instruções em [streamlit/README.md](./streamlit/readme.md).
*   **Treinamento do Modelo**: O modelo já vem treinado (`knn.joblib` e `scaler.joblib` em `modelo/`). Para explorar o processo de treinamento ou retreinar o modelo, utilize o notebook [modelo/classificacao_algoritmos.ipynb](./modelo/classificacao_algoritmos.ipynb).
*   **Coleta de Dados com ESP32**: Veja as instruções em [esp32/README.md](./esp32/readme.md).

## Estrutura do Diretório

```
.
├── api/                  # Aplicação FastAPI para a API de previsão
│   └── README.md
├── esp32/                # Código e diagramas para o ESP32
│   └── README.md
├── modelo/               # Notebooks, modelos treinados e scalers
│   └── classificacao_algoritmos.ipynb
├── streamlit/            # Aplicação Streamlit para o dashboard
│   └── README.md
├── datasets/             # Conjuntos de dados utilizados
│   └── README.md
├── requirements.txt      # Dependências do projeto Python
└── README.md             # Este README principal
```

---

## 📋 Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">Fiap</a> está licenciado sobre <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.</p>
