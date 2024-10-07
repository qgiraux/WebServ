import json, requests, cgi

form = cgi.FieldStorage()

w_city = form.getvalue('select')
if w_city is None:
	w_city = "paris"

def coordinates(city: str)->dict:
	cities = {
	"paris":{
		"longitude":"2.33",
		"latitude":"48.866667"
	},
	"tokyo":{
		"longitude":"139.691706",
		"latitude":"35.689487"
	},
	"new-york":{
		"longitude":"-73.954468",
		"latitude":"40.711874"
	}
	}
	if city not in cities:
		city = "paris"
	return cities[city]


url = f"https://api.open-meteo.com/v1/forecast?\
latitude={coordinates('tokyo').get('latitude')}\
&longitude={coordinates('tokyo').get('longitude')}\
&hourly=apparent_temperature,weather_code\
&forecast_days=1"




def	error():
	body = """
	<!DOCTYPE html>
	<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Document</title>
	</head>
	<body>
	<h1>Sorry something wnt wrong!<\h1>
	</body>
	</html>
		"""
	header = "Content-type: text/html\r\n"
	print(f"{header}\r\n{body}",end='\r\n')


# Ouverture des fichiers utils
try:
	with open("weather_codes.json", 'r') as file:
		try:
			weather_codes = json.load(file)
		except Exception as e:
			error()
			exit()
	if not isinstance(weather_codes, dict):
		error()
		exit()
except FileNotFoundError:
	error()
	print("")
	exit()


# requet a openweather et verification du json

response = requests.get(url)
if response.status_code != 200:
	error()
	exit()
weather_infos = response.json()
if not "hourly" in weather_infos:
	error()
	exit()
hourly = weather_infos.get('hourly')
if not "time" in hourly or not "apparent_temperature" in hourly or not "weather_code" in hourly:
	error()
	exit()

# Fonctions templates

def temp(hour :int)->str:
	if hour < 0 or hour > 24:
		return "-"
	t = hourly.get('apparent_temperature')
	if t is None or len(t) < hour:
		return "-"
	return t[hour]

def description(hour: int)->str:
	if hour < 0 or hour > 24 or hour > len(hourly['weather_code']):
		return "-"
	code = str(hourly['weather_code'][hour])
	if not code in weather_codes:
		return "-"
	if not "day" in weather_codes[code]:
		return "-"
	if not "description" in weather_codes[code]["day"]:
		return "-"
	return weather_codes[code]["day"]["description"]

def image(hour: int)->str:
	if hour < 0 or hour > 24 or hour > len(hourly['weather_code']):
		return "-"
	code = str(hourly['weather_code'][hour])
	if not code in weather_codes:
		return "-"
	if not "day" in weather_codes[code]:
		return "-"
	if not "image" in weather_codes[code]["day"]:
		return "-"
	return weather_codes[code]["day"]["image"]

def render_template(file_path):
	try:
		with open(file_path, 'r') as template_file:
			template_content = template_file.read()
			return eval(f"f'''{template_content}'''")
	except  FileNotFoundError:
		return ""
##

# fin fonctions templates
header = "Content-type: text/html\r\n"
body = render_template('template.html')
if len(body) == 0:
	error()
	exit()

print(f"{header}\r\n{body}",end='\r\n')

