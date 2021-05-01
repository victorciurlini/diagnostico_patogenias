import dash
import dash_core_components as dcc
import dash_html_components as html
import plotly.express as px
import pandas as pd
import mysql.connector as connection

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

cnx = connection.connect(user='ciurlini',
                              password='casa2406',
                              host='127.0.0.1',
                              database='DATABASE_PATOGENOS',
                              auth_plugin='mysql_native_password')
query = "SELECT * FROM tabela_dashboard;"
df = pd.read_sql(query,cnx)

df['total'] = 1
df['dia'] = [d.date() for d in df['horario']]
df['hora'] = [d.time() for d in df['horario']]
df['total'] = 1
grafico_linear = df
# Dropa colunas que não serão utilizadas
grafico_linear.drop(['horario', 'nome', 'path_database'], axis = 1, inplace=True)
# Reordena dataframe
grafico_linear = grafico_linear[['dia', 'hora', 'classificacao', 'total']]
# Traransforma em timestamp 
grafico_linear['dia'] = pd.to_datetime(grafico_linear.dia)
grafico_linear.sort_values(by=['dia', 'hora'], inplace=True)

dia = set(grafico_linear['dia'])
grafico_linear_agregado = df.groupby(grafico_linear['dia']).sum()
grafico_linear_agregado['dia'] = dia


classes = set(df['classificacao'])
tabela_final = df.groupby(df['classificacao']).sum()
tabela_final['classificacao'] = classes

# ['nome', 'horario', 'classificacao', 'path_database']
# assume you have a "long-form" data frame
# see https://plotly.com/python/px-arguments/ for more options
# df = pd.DataFrame({
#     "Fruit": ["Apples", "Oranges", "Bananas", "Apples", "Oranges", "Bananas"],
#     "Amount": [4, 1, 2, 2, 4, 5],
#     "City": ["SF", "SF", "SF", "Montreal", "Montreal", "Montreal"]
# })

fig = px.bar(tabela_final, x="classificacao", y="total", barmode="group")
graf_2 = px.line(grafico_linear_agregado, x="dia", y="total", line_shape="spline", render_mode="svg")

app.layout = html.Div(children=[
    html.H1(children='DashBoard: Doenças Patógenas'),

    html.Div([
        html.Div([
            html.H3('Volume de doenças encontradas'),
            dcc.Graph(id='g1', figure=fig)
        ], className="six columns"),

        html.Div([
            html.H3('As decisões da minha vida'),
            dcc.Graph(id='g2', figure=graf_2)
        ], className="six columns"),
    ], className="row")

])

if __name__ == '__main__':
    app.run_server(debug=True)

# app.layout = html.Div([
#     html.Div([
#         html.Div([
#             html.H3('Column 1'),
#             dcc.Graph(id='g1', figure={'data': [{'y': [1, 2, 3]}]})
#         ], className="six columns"),

#         html.Div([
#             html.H3('Column 2'),
#             dcc.Graph(id='g2', figure={'data': [{'y': [1, 2, 3]}]})
#         ], className="six columns"),
#     ], className="row")
# ])