using System.Collections;
using System.Diagnostics.CodeAnalysis;

namespace OOP_Lab09
{
    internal class Books<Tkey, Tvalue> : IDictionary<Tkey, Tvalue>
    {
        private List<KeyValuePair<Tkey, Tvalue>> _keyValuePairsList = [];

        public Tvalue this[Tkey key]
        {
            get
            {
                if (!ContainsKey(key))
                {
                    throw new KeyNotFoundException($"{key}");
                }
                TryGetValue(key, out var tvalue);

                return tvalue;
            }
            set
            {
                int index = FindIndexOf(key);

                if (index == -1)
                {
                    Add(key, value);
                }
                else
                {
                    _keyValuePairsList[index] = new KeyValuePair<Tkey, Tvalue>(key, value);
                }
            }
        }

        public ICollection<Tkey> Keys => (ICollection<Tkey>)_keyValuePairsList.Select(p => p.Key);

        public ICollection<Tvalue> Values => (ICollection<Tvalue>)_keyValuePairsList.Select(p => p.Value);

        public int Count => _keyValuePairsList.Count;

        public bool IsReadOnly => false;

        public void Add(Tkey key, Tvalue value)
        {
            Add(new KeyValuePair<Tkey, Tvalue>(key, value));
        }

        public void Add(KeyValuePair<Tkey, Tvalue> item)
        {
            if (ContainsKey(item.Key))
            {
                throw new ArgumentException($"{item.Key} уже есть в списке.");
            }
            _keyValuePairsList.Add(new KeyValuePair<Tkey, Tvalue>(item.Key, item.Value));
        }

        public void Clear() => _keyValuePairsList.Clear();

        public bool Contains(KeyValuePair<Tkey, Tvalue> item) =>
            _keyValuePairsList.Any(p => EqualityComparer<Tkey>.Default.Equals(p.Key, item.Key)
            && EqualityComparer<Tvalue>.Default.Equals(p.Value, item.Value));

        public bool ContainsKey(Tkey key) => _keyValuePairsList.Any(p => EqualityComparer<Tkey>.Default.Equals(p.Key, key));

        public void CopyTo(KeyValuePair<Tkey, Tvalue>[] array, int arrayIndex)
        {
            if (arrayIndex < 0)
            {
                throw new ArgumentOutOfRangeException($"{arrayIndex} не может быть меньше нуля");
            }

            _keyValuePairsList.CopyTo(array, arrayIndex);
        }

        public IEnumerator<KeyValuePair<Tkey, Tvalue>> GetEnumerator()
        {
            return _keyValuePairsList.GetEnumerator();
        }

        public void Remove(Tkey key)
        {
            var item = _keyValuePairsList.FirstOrDefault(p => EqualityComparer<Tkey>.Default.Equals(p.Key, key));

            if (!item.Equals(default(KeyValuePair<Tkey, Tvalue>)))
            {
                _keyValuePairsList.Remove(item);
            }
        }

        public bool Remove(KeyValuePair<Tkey, Tvalue> item)
        {
            return _keyValuePairsList.Remove(item);
        }

        public bool TryGetValue(Tkey key, [MaybeNullWhen(false)] out Tvalue value)
        {
            var item = _keyValuePairsList.FirstOrDefault(t => EqualityComparer<Tkey>.Default.Equals(t.Key, key));

            if (!EqualityComparer<Tkey>.Default.Equals(item.Key, default(Tkey)))
            {
                value = item.Value;
                return true;
            }

            value = default(Tvalue);
            return false;
        }

        public int FindIndexOf(Tkey key)
        {
            return _keyValuePairsList.FindIndex(p => EqualityComparer<Tkey>.Default.Equals(p.Key, key));
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        bool IDictionary<Tkey, Tvalue>.Remove(Tkey key)
        {
            int index = FindIndexOf(key);

            if (index == -1)
            {
                return false;
            }

            _keyValuePairsList.RemoveAt(index);

            return true;
        }
    }
}

