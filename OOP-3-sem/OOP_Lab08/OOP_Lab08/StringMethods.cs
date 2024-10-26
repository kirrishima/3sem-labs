namespace OOP_Lab08
{
    public class StringMethods
    {
        public static string RemoveElement(string str)
        {
            char[] element = { '.', ',', '!', '?', '-', ':' };
            for (var i = 0; i < str.Length; i++)
            {
                if (element.Contains(str[i]))
                {
                    str = str.Remove(i, 1);
                }
            }
            return str;
        }

        public static string RemoveSpaсe(string str) => str.Replace(" ", string.Empty);

        public static string Upper(string str)
        {
            for (var i = 0; i < str.Length; i++)
            {
                str = str.Replace(str[i], char.ToUpper(str[i]));
            }
            return str;
        }

        public static string Lower(string str)
        {
            for (var i = 0; i < str.Length; i++)
            {
                str = str.Replace(str[i], char.ToLower(str[i]));
            }
            return str;
        }

        public static string AddToString(string str) => str += " Coca Cola";
    }
}
