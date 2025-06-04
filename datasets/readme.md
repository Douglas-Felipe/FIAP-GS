# Projeto de Previsão de Enchentes na América do Sul

Este repositório contém o dataset (`dataset.csv`) e informações sobre o processo de desenvolvimento de um modelo de previsão de enchentes focado na América do Sul.

## Descrição do Dataset (`dataset.csv`)

O arquivo `dataset.csv` é o resultado de um processo de coleta e tratamento de dados de enchentes e informações meteorológicas. Ele foi estruturado para treinar um modelo de aprendizado de máquina capaz de prever a ocorrência de enchentes com base em dados de precipitação.

As colunas presentes no dataset são:

*   **Event_Date**: Data em que o evento de enchente foi registrado.
*   **Weather_Date**: Data para a qual o dado de precipitação foi coletado. Isso inclui o dia do evento (`Event_Date`) e os 6 dias anteriores.
*   **Latitude**: Latitude da localização do evento de enchente.
*   **Longitude**: Longitude da localização do evento de enchente.
*   **Precipitation**: Quantidade de precipitação registrada (em mm) para a `Weather_Date` e localização correspondente.
*   **Flood**: Variável alvo. Indica `1` se ocorreu uma enchente associada ao `Event_Date` (considerando os dados de precipitação até `Weather_Date`), e `0` caso contrário (representando dados do ano anterior na mesma data sem registro de enchente).

## Processo de Captura de Dados

A construção deste dataset envolveu duas fontes principais de informação e um processo de enriquecimento:

1.  **Dados de Enchentes:**
    *   **Fonte:** The International Charter Space and Major Disasters
    *   **Escopo:** Foram coletados dados de enchentes ocorridas na América do Sul no período entre 2000 e 2025. Esses dados incluíram a data e a localização (latitude e longitude) dos eventos.

2.  **Dados de Precipitação:**
    *   **Fonte:** Open-Meteo API
    *   **Metodologia:** Com base nos dados de enchentes obtidos, foi desenvolvido um script para consultar a API do Open-Meteo. Este script buscou os dados de precipitação diária para a localização de cada enchente, abrangendo:
        *   O dia exato da ocorrência da enchente.
        *   Os 6 dias anteriores ao evento.

## Abordagens de Modelagem e Geração de Dados de Treinamento

Durante o desenvolvimento, foram realizados testes para determinar a melhor forma de estruturar os dados para o treinamento do modelo. As duas abordagens principais consideradas foram:

1.  **Abordagem 1 (Sazonal - Escolhida):**
    *   **Descrição:** Utilizar os dados de precipitação dos 7 dias (dia do evento + 6 dias anteriores) para cada enchente registrada. Como contrapartida (casos de "não enchente" ou `Flood = 0`), foram utilizados os dados de precipitação para as mesmas datas e locais, porém do ano anterior, assumindo que não houve enchente nesses períodos.
    *   **Exemplo no `dataset.csv`:**
        ```
        Event_Date,Weather_Date,Latitude,Longitude,Precipitation,Flood
        2025-03-07,2025-03-07,-38.69888888888889,-62.33722222222222,79.4000015258789,1  // Enchente
        ...
        2025-03-07,2024-03-07,-38.69888888888889,-62.33722222222222,1.2999999523162842,0 // Mesmo dia, ano anterior (não enchente)
        ```
    *   **Resultado:** Esta abordagem apresentou o melhor desempenho nos testes.

2.  **Abordagem 2 (Eventos Aleatórios de Não Enchente):**
    *   **Descrição:** Utilizar os dados de precipitação dos dias de enchente e, para os casos de "não enchente", selecionar aleatoriamente outros dias ao longo do ano em que não foram registradas enchentes.
    *   **Resultado:** Esta abordagem foi descartada. Devido à grande disparidade entre a quantidade de dias com enchente e dias sem enchente, o modelo treinado com esses dados aprendeu predominantemente a prever a ausência de enchentes, não conseguindo generalizar para os casos positivos.

## Características e Limitações do Modelo Resultante (Baseado na Abordagem 1)

O modelo treinado com a primeira abordagem demonstrou ser eficaz, mas possui uma característica importante:

*   **Previsão Sazonal:** O modelo tende a ser mais preciso na previsão de enchentes que ocorrem de forma sazonal, ou seja, eventos que são comuns em épocas específicas do ano (como períodos de chuva intensa). Isso é uma consequência direta da forma como os dados de "não enchente" foram selecionados (ano anterior na mesma data).

*   **Relevância para o MVP:** Para o escopo de um Produto Mínimo Viável (MVP) e considerando o modelo de negócio focado na previsão de enchentes com base em precipitações, essa característica foi considerada aceitável. O objetivo principal é alertar sobre riscos de enchentes impulsionadas por padrões de chuva, e o modelo atual cumpre essa função, especialmente para eventos com precedentes sazonais.

## Estrutura do `dataset.csv`

O dataset final (`dataset.csv`) contém as seguintes colunas:

*   `Event_Date`: Data da ocorrência da enchente (ou data de referência para o ano anterior).
*   `Weather_Date`: Data específica da medição da precipitação (dia do evento ou um dos 6 dias anteriores).
*   `Latitude`: Latitude da ocorrência.
*   `Longitude`: Longitude da ocorrência.
*   `Precipitation`: Valor da precipitação em mm.
*   `Flood`: Variável binária indicando a ocorrência de enchente (`1` para enchente, `0` para não enchente).

---

Este README descreve o processo de coleta e a natureza dos dados utilizados. Para mais detalhes sobre o modelo, consulte o [jupyter notebook](../modelo/classificacao_algoritmos.ipynb).