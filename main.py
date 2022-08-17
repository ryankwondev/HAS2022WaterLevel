from fastapi import FastAPI, Response, status
from pydantic import BaseModel
import datetime as dt
import psycopg2

app = FastAPI()


class MeasuredData(BaseModel):
    auth_key: str
    level: int


@app.get("/")
async def root():
    return {"message": "Hello World"}


@app.post("/measurements/{measurement_id}", status_code=200)
async def create_measurement(measurement_id: int, data: MeasuredData, response: Response):
    if data.auth_key != "p5ssw0rd":
        response.status_code = status.HTTP_401_UNAUTHORIZED
        return {"detail": "Invalid auth key"}
    elif data.level < 0 or data.level > 1023:
        response.status_code = status.HTTP_400_BAD_REQUEST
        return {"detail": "Invalid level"}

    timestamp = dt.datetime.now(dt.timezone("Asia/Seoul"))

    connection = psycopg2.connect(dbname="postgres", user="postgres", password="p5ssw0rd", host="localhost")
    cursor = connection.cursor()
    cursor.execute("INSERT INTO measurements (measurement_id, level, created_at) VALUES (%s, %s, %s);",
                   (measurement_id, data.level, timestamp))
    connection.commit()
    connection.close()

    return {"measurement_id": measurement_id, "level": data.level, "created_at": timestamp}


@app.get("/measurements/{measurement_id}", status_code=200)
async def get_measurement(measurement_id: int, response: Response):
    connection = psycopg2.connect(dbname="postgres", user="postgres", password="p5ssw0rd", host="localhost")
    cursor = connection.cursor()
    cursor.execute("SELECT * FROM measurements WHERE measurement_id = %s ORDER BY created_at LIMIT 30;",
                   (measurement_id,))
    result = cursor.fetall()
    connection.close()

    if result is None:
        response.status_code = status.HTTP_404_NOT_FOUND
        return {"detail": "Measurement not found"}

    return result
