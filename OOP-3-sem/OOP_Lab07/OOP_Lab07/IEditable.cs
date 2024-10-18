namespace OOP_Lab07
{
    internal interface IEditable<T>
    {
        void Add(T item);

        void Delete(int index);

        void Show();
    }
}
