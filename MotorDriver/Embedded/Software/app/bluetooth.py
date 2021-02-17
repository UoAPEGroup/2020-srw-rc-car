#dash/plotly 
import dash
import asyncio
from dash.dependencies import Output, Input, State
import dash_bootstrap_components as dbc
import dash_core_components as dcc
import dash_html_components as html
import plotly
import plotly.graph_objs as go
from plotly.subplots import make_subplots

#misc
import time
from datetime import datetime

from collections import deque
import psycopg2

#local files
from reuse import *

#define SQL to read from online database
global readNewestCarVCS, readNewestCarDS

readNewestCarVCS = """ 
            SELECT voltage, current FROM carVCS WHERE id = %s
            """
readNewestCarDS = """
            SELECT direction, speedGrade, overVoltage, overCurrent, establishedConnection FROM carDS WHERE id = 0
                """

def openDatabaseConnection():
    con = None

    try: 
        con = psycopg2.connect(database = "d4odufthffmam6", user = "rhahsgujdzpjxa", port = "5432", 
                                    password = "2fe675581f9680a9958f466cd1953b7fecaea654271a9d6f614250e36bf389fc",
                                    host = "ec2-54-164-233-77.compute-1.amazonaws.com")
    except Exception as err:
        print("Couldn't connect to database")

    return con

def updateValues(cur, currentId):
    global systemParameters

    cur.execute(readNewestCarVCS, (currentId,))

    try: 
        values = cur.fetchall()

        if (values != None):
            voltage = values[0][0]/1000
            current = values[0][1]/1000
            print(voltage)

            power = voltage * current

            voltageValues.append(voltage)
            currentValues.append(current)
            powerValues.append(power)

            voltageMax.append(20)
            
            X.append(X[-1] + 1)
            X2.append(X2[-1] + 1)
            X3.append(X3[-1] + 1)

        cur.execute(readNewestCarDS)
    
        values = cur.fetchall()
        count = 0

        for value in values:
            systemParameters[count] = value
            count = count + 1

    except Exception: 
        pass

def returnDirectionAndSpeedString(direction, speedGrade):
    directionAndSpeedString = []

    directionString = {
        0: "Forward",
        1: "Reverse"
    }

    speedString = {
        0: "Stopped",
        1: "Min",
        2: "Mid",
        3: "Max"
    }

    directionAndSpeedString.append(directionString.get(direction, "Invalid"))
    directionAndSpeedString.append((speedString.get(speedGrade, "Invalid")))

    return directionAndSpeedString

def initializeLists():
    global X, X2, X3, X4, voltageValues, voltageMax, currentValues, powerValues, speedValues

    X = deque(maxlen = 20)
    X.append(0)
    voltageValues = deque(maxlen = 20)
    voltageValues.append(0)

    voltageMax = deque(maxlen = 20)
    voltageMax.append(20)

    X2 = deque(maxlen = 20)
    X2.append(0)
    currentValues = deque(maxlen = 20)
    currentValues.append(0)

    X3 = deque(maxlen = 20)
    X3.append(0)
    powerValues = deque(maxlen = 20) 
    powerValues.append(0)

    X4 = deque(maxlen = 20)
    X4.append(0)
    speedValues = deque(maxlen = 20)
    speedValues.append(0)

bluetooth = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP], update_title='Updating...')

startTime = time.time()

#initialize lists that will be the x and y values of graphs
initializeLists()

global maxID, systemParameters
systemParameters = []

#open connection to database
con = openDatabaseConnection()

cur = con.cursor()

#extract initial voltage, current, and speed values
cur.execute(""" select * from carVCS order by id""")
rows = cur.fetchall()
for row in rows:
    voltage = row[1]/1000
    current = row[2]/1000
    power = voltage * current

    voltageValues.append(voltage)
    currentValues.append(current)
    powerValues.append(power)
    
    voltageMax.append(20)

    X.append(X[-1] + 1)
    X2.append(X2[-1] + 1)
    X3.append(X3[-1] + 1)

#extract initial system parameters (direction, speedGrade, and safety parameters)
cur.execute(""" SELECT * FROM carDS """)
rows = cur.fetchall()

count = 0
for row in rows:
    systemParameters.append([row][count])
    count = count + 1

for parameter in systemParameters:
    print(parameter)

#identify the largest ID in the system (i.e., the last transmission sent by the uC)
cur.execute("""SELECT MAX(id) FROM carVCS""")

maxID = cur.fetchone()
maxID = maxID[0]

#layout for the webpage
bluetooth.layout = dbc.Container(

    children = [

        #create the header
        html.Div( 
            dbc.Container(
                dbc.Jumbotron(
                html.Div(
                    children = [
                        html.Div (
                            children = [
                            html.H1('RC Car Live Updates', className = "display-4", style = {"marginTop": "28vw", 'paddingTop': "1vw"}),
                            html.Hr(style = {"margin-right": "57vw", "backgroundColor": "white"}),
                            html.P('UoA FoE Summer Workshop 2020', className = "lead", style = {"margin-top": "2vw"})], style = {}),
                        html.Img(src = bluetooth.get_asset_url("uoaSquare.png"), style = {"height": "10vw", "paddingLeft": "78vw", "marginTop": "-3vw"}),
                ], style = {"marginLeft": "5vw", "color": "white"} ), fluid = True, style = {"backgroundImage": "linear-gradient(rgba(0,0,0,0.5), rgba(0,0,0,0.5)), url(https://images.unsplash.com/photo-1465447142348-e9952c393450?ixid=MXwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHw%3D&ixlib=rb-1.2.1&auto=format&fit=crop&w=1568&q=80)",
                "backgroundPosition": "center center", "backgroundSize": "cover", "height": "45vw", "marginLeft": "0"}),
            fluid = True),
        ),
        
        html.Div( [
            dbc.Row(dbc.Col(dbc.Jumbotron(html.H1("Live Graphs", className = "display-4", style = {}), style = {"height": "10vw", "marginTop": "2vw", "marginLeft": "2vw", "backgroundColor": "#FAFAFA"}), width = 10), style = {} ),
            dbc.Row( 
                [ 
                    dbc.Col( dcc.Graph( id = 'carData', animate = False), width = 10), 
                    dbc.Col( children = [
                        dbc.Row(dbc.Card( children = [dbc.CardBody(id = 'direction', className = "card-title", style = {"textAlign": "left"})], color = uoaDarkBlue, inverse = True, style = {"marginTop": "6vw"})),
                        dbc.Row([
                            dbc.Button("System Ratings:", outline = False, id = "collapseButtonSR", style = {"backgroundColor": "#FAFAFA", "color": "black", "marginTop": "1vw"}), 
                            dbc.Collapse(parameterTable, id = "collapseSR", style = {"marginTop": "0.5vw", "marginLeft": "0.2vw"})
                        ]),
                        dbc.Row(
                            id = "safety",
                        ),
                        ], width = 2),
                    

                ],
            ),
        ],
        ),

        dcc.Interval(
            id='updateGraph',
            interval = 1*800,
        ),

        ],
    fluid = True, style = {"backgroundColor" : "#FAFAFA"},
)

#system callbacks

@bluetooth.callback(Output("collapseSR", "is_open"), 
                    [Input("collapseButtonSR", "n_clicks")], 
                    [State("collapseSR", "is_open")],)
def toggleCollapseSR(n, is_open): 
    if n:
        return not is_open
    return is_open

@bluetooth.callback(Output('direction', 'children'),
                    Input('updateGraph', 'n_intervals'))
def metric_update(self):
    global maxID, directionAndSpeedString, systemParameters

    ''' check if there's been an update to the database'''

    cur.execute("SELECT MAX(id) FROM carVCS")

    try: 
        currentId = cur.fetchone()

        if currentId != None: 
            print(currentId[0])

            currentId = currentId[0]

            if ((currentId != None) and (currentId > maxID)):
                maxID = currentId
                updateValues(cur, maxID)

            directionAndSpeedString = returnDirectionAndSpeedString(systemParameters[0][1], systemParameters[0][1])

    except Exception: 
        pass

    return [
        html.Div(
            children = [
            html.H2("Direction: ", className = "display-5", style = {"fontSize": "1.5vw"}),
            html.Hr(style = {"backgroundColor": "white", "marginTop": "-0.3vw"}),
            html.H4(directionAndSpeedString[0], className = "display-4", style = {"fontSize": "2.5vw", "marginTop": "-0.7vw"}), 
            html.H2("Speed: ", className = "display-5", style = {"fontSize": "1.5vw", "marginTop": "2vw"}),
            html.Hr(style = {"backgroundColor": "white", "marginTop": "-0.3vw"}),
            html.H4(directionAndSpeedString[1], style = {"fontSize": "2.5vw", "marginTop": "-0.7vw"}, className = "display-4")],
        ),
    ]

@bluetooth.callback(Output('safety', 'children'),
                    Input('updateGraph', 'n_intervals'))
def update_safety(self):
    global systemParameters

    voltageColor = "success"
    currentColor = "success"
    establishedConnection = "danger"

    if systemParameters[0][2]:
        voltageColor = "danger"
    if systemParameters[0][3]:
        currentColor = "danger"
    if systemParameters[0][4]:
        establishedConnection = "success"

    return [
        dbc.ButtonGroup ([
        dbc.Button("OverVoltage", color = voltageColor, style = {"marginTop": "1vw"}), 
        dbc.Button("OverCurrent", color = currentColor, style = {'marginTop': "0.5vw"}),
        dbc.Button("Connection Established", color = establishedConnection, style = {"marginTop": "0.5vw"})
        ], vertical = True, style = {}
        ),
    ]

@bluetooth.callback(Output('carData', 'figure'),
                    [Input('updateGraph', 'n_intervals')])
def update_graph(self):

    fig = make_subplots(
        rows = 2, cols = 2, 
        vertical_spacing = 0.2, 
        subplot_titles = ("Voltage at H-Bridge vs Time", "Current at H-Bridge vs Time", "Motor Power vs. Time", "Car Speed vs. Time"),
    )

    fig.add_trace(
        go.Scatter(
            x = list(X),
            y = list(voltageValues),  
            name = 'Voltage', 
            mode = 'lines+markers',

        ),
        row = 1, col = 1)

    fig.add_trace(
        go.Scatter(
            x = list(X),
            y = list(voltageMax),
            mode = 'lines',
            name = 'Max Voltage'
        ),
        row = 1, col = 1)
    
    fig.add_trace( 
        go.Scatter(
            x = list(X2),
            y = list(currentValues), 
            name = 'Current',
            mode = 'lines+markers',
        ),
        row = 1, col = 2)

    fig.add_trace(
        go.Scatter(
            x = list(X3),
            y = list(powerValues),
            name = 'Power',
            mode = 'lines+markers',
        ),
        row = 2, col = 1
    ) 

    fig.add_trace(
        go.Scatter(
            x = list(X4),
            y = list(speedValues),
            name = "Speed",
            mode = "lines+markers"
        ),
        row = 2, col = 2
    )

    fig.update_xaxes(title_text = "time(s)", gridcolor = lightGray, range = [min(X) - 1, max(X) + 1], row = 1, col = 1)
    fig.update_xaxes(title_text = "time(s)", gridcolor = lightGray, range = [min(X2) - 1, max(X2) + 1], row = 1, col = 2)
    fig.update_xaxes(title_text = "time(s)", gridcolor = lightGray, range = [min(X3) - 1, max(X3) + 1], row = 2, col = 1)
    fig.update_xaxes(title_text = "time(s)", gridcolor = lightGray, range = [min(X4) - 1, max(X4) + 1], row = 2, col = 2)

    axesCoefficient = 0.1

    fig.update_yaxes(title_text = "Voltage(V)", gridcolor = lightGray, range = [min(voltageValues) - axesCoefficient*(min(voltageValues)), max(voltageValues) + axesCoefficient*(max(voltageValues))], row = 1, col = 1)
    fig.update_yaxes(title_text = "Current(A)", gridcolor = lightGray, range = [min(currentValues) - axesCoefficient *(min(currentValues)), max(currentValues) + axesCoefficient * (max(currentValues))], row = 1, col = 2)
    fig.update_yaxes(title_text = "Power(W)", gridcolor = lightGray, range = [min(powerValues) - axesCoefficient *(min(powerValues)), max(powerValues) + axesCoefficient * (max(powerValues))], row = 2, col = 1)
    fig.update_yaxes(title_text = "Speed(m/s)", gridcolor = lightGray, range = [min(speedValues) - axesCoefficient * (min(speedValues)), max(speedValues) + axesCoefficient * (max(speedValues))], row = 2, col = 2)

    for i in fig['layout']['annotations']:
        i['font'] = dict(size=18)

    fig.update_layout(paper_bgcolor = "white", plot_bgcolor = "white", height = 800, font_color = textColor, showlegend = True),

    return fig

if __name__ == '__main__':    
    
    bluetooth.run_server(debug=True, use_reloader = False)
    cur.close()
    con.close()

    print("closed")
    