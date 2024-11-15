using System.Reflection;
using System.Text;

namespace OOP_Lab11
{
    public static class Reflector
    {
        public static string? GetAssemblyName(Type type) => type.Assembly.GetName().Name;

        public static bool HasPublicConstructors(Type type) => type.GetConstructors().Any(c => c.IsPublic);

        public static IEnumerable<string> GetMethods(Type type) => type.GetMethods().Where(m => m.IsPublic).Select(i => i.Name);

        public static IEnumerable<string> GetInterfaces(Type type) => type.GetInterfaces().Select(i => i.ToString());

        public static IEnumerable<string?> GetFieldsAndProps(Type type) =>
            type.GetFields().Select(f => f.ToString()).Concat(type.GetProperties().Select(p => p.ToString()));

        public static void PrintAllMethodsContainingParamsWithType(Type type, Type param)
        {
            var methods = type.GetMethods()
                .Where(m => m.GetParameters()
                .Any(p => p.ParameterType == param));

            foreach (var item in methods)
            {
                Console.WriteLine(item);
            }
        }

        public static object? Create(Type type, params object[] data) => Activator.CreateInstance(type, data);

        public static object? Invoke(object obj, string methodName, object[] parameters)
        {
            var type = obj.GetType();

            var method = type.GetMethod(methodName, parameters.Select(p => p.GetType()).ToArray());

            if (method == null)
            {
                throw new ArgumentException($"Метод {methodName} не найден в классе {type.Name}.");
            }

            return method?.Invoke(obj, parameters);
        }

        public static object[] GetParametersFromFileOrGenerate(string filePath, MethodInfo method)
        {
            var parameters = method.GetParameters();
            object[] values = new object[parameters.Length];

            if (File.Exists(filePath))
            {
                var lines = File.ReadAllLines(filePath);

                for (int i = 0; i < parameters.Length; i++)
                {
                    var paramType = parameters[i].ParameterType;

                    var line = lines[i];
                    if (line != null)
                    {
                        var value = line.Split('=')[1];

                        values[i] = Convert.ChangeType(value, paramType);
                    }
                    else
                    {
                        values[i] = GenerateValueForType(paramType);
                    }
                }
            }
            else
            {
                using (StreamWriter sw = new StreamWriter(filePath, false, Encoding.UTF8))
                {
                    for (int i = 0; i < parameters.Length; i++)
                    {
                        values[i] = GenerateValueForType(parameters[i].ParameterType);
                        sw.WriteLine($"{parameters[i].ParameterType.FullName}={values[i]}");
                    }
                    sw.Flush();
                }
            }

            return values;
        }

        private static object GenerateValueForType(Type type)
        {
            if (type == typeof(int))
                return new Random().Next(1, 100);
            if (type == typeof(string))
                return "sample string";
            if (type == typeof(double))
                return new Random().NextDouble() * 100;
            if (type == typeof(bool))
                return new Random().Next(0, 2) == 0;

            return null;
        }
    }
}
