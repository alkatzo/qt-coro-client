from flask import Flask, request, jsonify
from datetime import datetime
import time  # Import the time module

app = Flask(__name__)

# Sample data
people_data = [
    {"firstName": "John", "lastName": "Doe", "dateOfBirth": "1980-01-01"},
    {"firstName": "Jane", "lastName": "Smith", "dateOfBirth": "1990-02-02"},
    {"firstName": "Alice", "lastName": "Johnson", "dateOfBirth": "1975-03-03"},
    {"firstName": "Bob", "lastName": "Brown", "dateOfBirth": "1985-04-04"},
    {"firstName": "Charlie", "lastName": "Davis", "dateOfBirth": "1992-05-05"},
]

@app.route('/v1/people', methods=['GET'])
def get_people():
    time.sleep(1)  # simulate 3 sec delay

    datetime_str = request.args.get('datetime')
    page = request.args.get('page', type=int)

    # Validate datetime format
    try:
        datetime_obj = datetime.fromisoformat(datetime_str)
    except ValueError:
        return jsonify({"error": "Invalid date-time format"}), 400

    # Return all data if page is not provided
    if page is None:
        return jsonify(people_data), 200

    # Validate page number
    if page < 1:
        return jsonify({"error": "Invalid page number"}), 400

    # Define a page size for pagination
    PAGE_SIZE = 2

    # Paginate data
    start_index = (page - 1) * PAGE_SIZE
    end_index = start_index + PAGE_SIZE
    paginated_data = people_data[start_index:end_index]

    return jsonify(paginated_data), 200

if __name__ == '__main__':
    app.run(host='172.29.208.1', port=5000)
