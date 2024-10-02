import os
import re

def rename_files():
    current_dir = 'Лабы'
    for file_name in os.listdir(current_dir):
        if file_name.endswith(('.pdf', '.docx')):
            numbers = re.findall(r'\d+', file_name)
            if numbers:
                old_file_path = os.path.join(current_dir, file_name)
                file_extension = os.path.splitext(file_name)[1]
                new_file_name = f"Лр {numbers[1]}" + file_extension
                new_file_path = os.path.join(current_dir, new_file_name)
                os.rename(old_file_path, new_file_path)


def rename_files_2():
    current_dir = 'Презентации'
    for file_name in os.listdir(current_dir):
        if file_name.endswith('.pptx'):
            new_file_name = re.sub(r'семестр\s*2\s*.', '', file_name, flags=re.IGNORECASE)
            new_file_name = re.sub(r'\(\d+\)', '', new_file_name)
            new_file_name = re.sub(r'\s+', ' ', new_file_name).strip()
            old_file_path = os.path.join(current_dir, file_name)
            new_file_path = os.path.join(current_dir, new_file_name)

            os.rename(old_file_path, new_file_path)


rename_files()
rename_files_2()
