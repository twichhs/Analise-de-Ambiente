from flask import Flask, request, jsonify
import pandas as pd
import os

app = Flask("espAPI")
arquivo = "registro_alertas.csv"

if not os.path.exists(arquivo):
    df = pd.DataFrame(columns=["temp","umidade","distancia","estado"])
    df.to_csv(arquivo, index=False)

@app.route("/dados", methods=["POST"])
def receber_dados():
    data = request.get_json()

    nova_linha = pd.DataFrame([data])
    nova_linha.to_csv(arquivo, mode="a", header=False, index=False)

    return jsonify({"status": "ok"})

app.run(host="0.0.0.0", port=5000)