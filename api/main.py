import os
import joblib
import pandas as pd

from typing import List
from fastapi import FastAPI, Response

app = FastAPI()

# =============================
# Carregar modelo e scaler
# =============================
modelo_path = "../modelo/knn.joblib"
scaler_path = "../modelo/scaler.joblib"

@app.get("/")
def read_root():
    return {"message": "API de previsão funcionando!"}

@app.post("/predict")
def predict(data: List[float], response: Response):
    if not (os.path.exists(modelo_path) and os.path.exists(scaler_path)):
        response.status_code = 500
        return { "erro": "modelo não encontrado" }
    
    # =============================
    # Features
    # =============================
    epsilon = 1e-6  # Evitar divisão por zero
    precip_series = pd.Series(data[0:7])

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
    
    # =============================
    # Escalonamento e Previsão
    # =============================
    modelo = joblib.load(modelo_path)
    scaler = joblib.load(scaler_path)
    
    scaled = scaler.transform(features)
    prediction = modelo.predict(scaled)

    return {"enchente": True if prediction[0] == 1 else False}
