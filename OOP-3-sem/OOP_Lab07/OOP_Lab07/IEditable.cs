namespace OOP_Lab07
{
    internal interface IEditable<T>
    {
        void Add(T item);

        void Delete(T item);

        void Show(T item);
    }
}
