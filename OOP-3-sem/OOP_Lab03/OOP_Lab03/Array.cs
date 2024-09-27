using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP_Lab03
{
    internal partial class Array
    {
        protected int[] _array;
        public Array(int size)
        {
            _array = new int[size];
        }

        public Array(Array other)
        {
            _array = new int[other.Length];
            System.Array.Copy(_array, other._array, Length);
        }

        public int[] Data { get { return _array; } }

        public int Length { get { return _array.Length; } }

        public static Array operator -(Array obj, int value)
        {
            Array tmp = new Array(obj);

            for (int i = 0; i < obj.Length; i++)
            {
                obj[i] -= value;
            }

            return tmp;
        }

        public int this[int index]
        {
            get
            {
                return _array[index];
            }
            set
            {
                _array[index] = value;
            }
        }

        public static bool operator >(Array obj, int value)
        {
            return obj._array.Contains(value);
        }

        public static bool operator <(Array obj, int value)
        {
            return !obj._array.Contains(value);
        }

        public static bool operator !=(Array obj, Array other)
        {
            if (other.Length != obj.Length)
            {
                return true;
            }

            for (int i = 0; i < obj.Length; i++)
            {
                if (obj[i] != other[i])
                {
                    return true;
                }
            }

            return false;
        }

        public static bool operator ==(Array obj, Array other)
        {
            if (other.Length != obj.Length)
            {
                return false;
            }

            for (int i = 0; i < obj.Length; i++)
            {
                if (obj[i] != other[i])
                {
                    return false;
                }
            }

            return true;
        }

        public static Array operator +(Array obj1, Array obj2)
        {
            Array array = new(obj1.Length + obj2.Length);

            System.Array.Copy(obj1._array, array._array, obj1._array.Length);
            System.Array.Copy(obj2._array, 0, array._array, obj1.Length, obj2.Length);

            return array;
        }

        public override string ToString()
        {
            return '[' + String.Join(", ", _array) + ']';
        }
    }
}
