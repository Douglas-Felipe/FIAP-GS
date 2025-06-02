import streamlit as st
import pandas as pd
import joblib
import os

# =============================
# Carregar modelo e scaler
# =============================
modelo_path = "../modelo/knn.joblib"
scaler_path = "../modelo/scaler.joblib"

if not (os.path.exists(modelo_path) and os.path.exists(scaler_path)):
    st.error("Modelo ou scaler não encontrados na pasta ./modelo/. Verifique os arquivos.")
    st.stop()

modelo = joblib.load(modelo_path)
scaler = joblib.load(scaler_path)

# =============================
# Interface Streamlit
# =============================
st.set_page_config(page_title="Previsão de Enchente")
st.title("Previsão de Enchente com Base Na Precipitação dos Últimos 7 Dias")

st.subheader("Insira os valores de precipitação (mm) para cada dia:")

dias = ["Dia atual", "1 dia antes", "2 dia antes", "3 dia antes", "4 dia antes", "5 dia antes", "6 dia antes"]

# Inputs para cada dia
precipitacoes = []
for dia in dias:
    valor = st.number_input(
        label=f"Precipitação {dia} (mm)",
        min_value=0.0,
        step=0.1,
        format="%.1f"
    )
    precipitacoes.append(valor)

# =============================
# Features
# =============================
epsilon = 1e-6  # Evitar divisão por zero
precip_series = pd.Series(precipitacoes)

precip_1d = precip_series.iloc[0]
precip_3d = precip_series.iloc[0:3].sum()
precip_7d = precip_series.sum()

ratio_1d_7d = precip_1d / (precip_7d + epsilon)
acceleration = precip_1d - (precip_3d / 3)
impact_score = (precip_1d ** 2) + (precip_3d ** 1.5) + precip_7d


features = pd.DataFrame([{
    'Precip_1d': precip_1d,
    'Precip_3d': precip_3d,
    'Precip_7d': precip_7d,
    'Ratio_1d_7d': ratio_1d_7d,
    'Acceleration': acceleration,
    'Impact_Score': impact_score
}])

st.subheader("Dados calculados para previsão:")
st.dataframe(features, hide_index=True)

with st.expander("Clique aqui para ver a explicação dos dados calculados"):
  explicacao_features = pd.DataFrame({
      "Feature": [
          "Precip_1d",
          "Precip_3d",
          "Precip_7d",
          "Ratio_1d_7d",
          "Acceleration",
          "Impact_Score"
      ],
      "Nome Amigável": [
          "Precipitação no Dia atual (mm)",
          "Precipitação nos Últimos 3 Dias (mm)",
          "Precipitação nos Últimos 7 Dias (mm)",
          "Proporção Dia atual / 7 Dias",
          "Aceleração da Precipitação",
          "Índice de Impacto"
      ],
      "Descrição": [
          "Total de precipitação apenas no dia mais recente.",
          "Soma da precipitação dos últimos 3 dias.",
          "Soma da precipitação total dos últimos 7 dias.",
          "Proporção entre a precipitação do dia atual e o total dos últimos 7 dias.",
          "Diferença entre a precipitação do dia atual e a média dos últimos 3 dias (indica aumento ou diminuição).",
          "Índice que combina diferentes fatores da chuva recente, ponderando eventos fortes e concentrados."
      ]
  })
  st.dataframe(explicacao_features, hide_index=True)

# =============================
# Previsão
# =============================
# Escalar os dados
features_scaled = scaler.transform(features)

# Fazer previsão
pred = modelo.predict(features_scaled)

# =============================
# Resultado
# =============================
if pred[0] == 1:
    st.error("🚨 **ALERTA:** Alto risco de enchente detectado! Tome as devidas precauções e acompanhe os avisos das autoridades.")
else:
    st.success("✅ **Sem risco de enchente.** Situação dentro dos parâmetros normais.")