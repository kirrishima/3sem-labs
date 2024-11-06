import os
import re

def rename_files(path):
    current_dir = path
    for file_name in os.listdir(current_dir):
        file_path = os.path.join(current_dir, file_name)

        if os.path.isdir(file_path):
            rename_files(file_path)
        elif os.path.isfile(file_path) and file_name.endswith(('.pptx', '.pdf', '.docx')):
            new_file_name = re.sub(r'_+', ' ', file_name)
            new_file_name = re.sub(r'\s+', ' ', new_file_name).strip()
            new_file_name = re.sub(r'\s+\.(?=\w+$)', '.', new_file_name)
            old_file_path = file_path
            new_file_path = os.path.join(current_dir, new_file_name)

            if old_file_path != new_file_path:
                os.rename(old_file_path, new_file_path)
                print(f'Renamed "{file_name}" to "{new_file_name}"')

rename_files(os.path.dirname(os.path.abspath(__file__)))
rename_files(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),  "Лабы"))
