import pandas as pd

df = pd.read_csv("registro_alertas.csv")

df["estado_calc"] = df["distancia"].apply(lambda x: "RISCO" if x < 0.04 else "SEGURO")

total = len(df)

risco = len(df[df["estado_calc"] == "RISCO"])
seguro = len(df[df["estado_calc"] == "SEGURO"])

porcentagem_risco = (risco / total) * 100 if total > 0 else 0

print("Total de leituras:", total)
print("Eventos de risco:", risco)
print("Eventos seguros:", seguro)
print("Porcentagem de risco:", porcentagem_risco)

criticos = df[df["distancia"] < 0.04]
print("\nCasos críticos:")
print(criticos)

if risco > 5:
    print("\nALERTA: alto número de eventos de risco no depósito")