using OOP_Lab07;
using System.Numerics;
using System.Text.Json;

namespace OOP_Lab03
{
    public partial class Array<T> : IEditable<T> where T : struct, ISubtractionOperators<T, T, T>
    {
        protected T[] _array;

        public T[] Data { get { return _array; } }

        public int Length { get { return _array.Length; } }

        public Array(int size)
        {
            _array = new T[size];
        }

        public Array(T[] arr)
        {
            _array = new T[arr.Length];
            System.Array.Copy(arr, _array, Length);
        }

        public Array(Array<T> other) : this(other._array) { }

        public override bool Equals(object? obj)
        {
            if (obj == null || this.GetType() != obj.GetType())
            {
                return false;
            }

            Array<T> other = (Array<T>)obj;

            if (this.Length != other.Length)
            {
                return false;
            }

            for (int i = 0; i < this.Length; i++)
            {
                if (this[i].Equals(other[i]))
                {
                    return false;
                }
            }

            return true;
        }

        public static Array<T> operator -(Array<T> obj, T value)
        {
            var tmp = new Array<T>(obj);

            for (int i = 0; i < obj.Length; i++)
            {
                obj[i] -= value;
            }

            return tmp;
        }

        public T this[int index]
        {
            get
            {
                if (index < 0 || index >= _array.Length)
                {
                    throw new ArgumentOutOfRangeException(nameof(index), $"Индекс {index} находится вне допустимого диапазона [{0}, {Length}].");
                }

                return _array[index];
            }
            set
            {
                if (index < 0 || index >= _array.Length)
                {
                    throw new ArgumentOutOfRangeException(nameof(index), $"Индекс {index} находится вне допустимого диапазона [{0}, {Length}].");
                }

                _array[index] = value;
            }
        }

        public static bool operator >(Array<T> obj, T value)
        {
            return obj._array.Contains(value);
        }

        public static bool operator <(Array<T> obj, T value)
        {
            return !(obj > value);
        }

        public static bool operator !=(Array<T> obj, Array<T> other)
        {
            return !obj.Equals(other);
        }

        public static bool operator ==(Array<T> obj, Array<T> other)
        {
            return obj.Equals(other);
        }

        public static Array<T> operator +(Array<T> obj1, Array<T> obj2)
        {
            Array<T> array = new(obj1.Length + obj2.Length);

            System.Array.Copy(obj1._array, array._array, obj1._array.Length);
            System.Array.Copy(obj2._array, 0, array._array, obj1.Length, obj2.Length);

            return array;
        }

        public void Add(T item)
        {
            T[] tmp = new T[Length + 1];

            System.Array.Copy(_array, tmp, Length);
            tmp[tmp.Length - 1] = item;

            _array = tmp;
        }

        public void Delete(int index)
        {
            if (index < 0 || index >= Length)
                throw new ArgumentOutOfRangeException(nameof(index));

            T[] tmp = new T[Length - 1];

            System.Array.Copy(_array, 0, tmp, 0, index);
            System.Array.Copy(_array, index + 1, tmp, index, Length - index - 1);

            _array = tmp;
        }

        public void Show()
        {
            Console.WriteLine(this.ToString());
        }

        public override string ToString()
        {
            return $"[{string.Join(", ", _array)}]";
        }

        public void SaveToFile(string filePath)
        {
            string jsonString = JsonSerializer.Serialize(this._array);
            File.WriteAllText(filePath, jsonString);
        }

        public static Array<T> LoadFromFile(string filePath)
        {
            if (!File.Exists(filePath))
                throw new FileNotFoundException($"Файл {filePath} не найден.");

            string jsonString = File.ReadAllText(filePath);
            T[] data = JsonSerializer.Deserialize<T[]>(jsonString);
            return new Array<T>(data);
        }
    }
}
