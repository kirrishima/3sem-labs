import os
import re

def rename_files():
    current_dir = 'Лабы'
    for file_name in os.listdir(current_dir):
        if file_name.endswith(('.pdf', '.docx')):
            # Ищем номер лабораторной работы
            lab_number_match = re.search(r'Лабораторная работа (\d+)', file_name)
            if not lab_number_match:
                lab_number_match = re.search(r'Лабоарторная работа (\d+)', file_name)
            if lab_number_match:
                lab_number = lab_number_match.group(1)
                # Ищем основное название файла после номера лабораторной работы
                main_title_match = re.search(r'Лабораторная работа \d+\.\s*(.*)\.', file_name)
                if not main_title_match:
                    main_title_match = re.search(r'Лабоарторная работа \d+\.\s*(.*)\.', file_name)
                if main_title_match:
                    main_title = main_title_match.group(1).strip()
                    
                    # Получаем расширение файла
                    file_extension = os.path.splitext(file_name)[1]
                    
                    # Формируем новое название
                    new_file_name = f"ЛР {lab_number}. {main_title}{file_extension}"
                    
                    # Переименовываем файл
                    old_file_path = os.path.join(current_dir, file_name)
                    new_file_path = os.path.join(current_dir, new_file_name)
                    os.rename(old_file_path, new_file_path)
                    print(f"Файл {file_name} переименован в {new_file_name}")


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
