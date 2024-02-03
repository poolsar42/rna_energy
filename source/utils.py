import json

def convert_txt_to_json(txt_file_path, json_file_path):
    data = {}

    with open(txt_file_path, 'r') as file:
        current_key = None
        for line in file:
            line = line.strip()
            if line and not line.endswith(':'):
                parts = line.split()
                current_key = parts[0]
                try:
                    values = [float(val) for val in parts[1:]]
                    data[current_key] = values
                except ValueError as e:
                    raise ValueError(f"Error processing line: {line}. {e}")

    with open(json_file_path, 'w') as json_file:
        json.dump(data, json_file, indent=2)