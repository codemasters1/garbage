File.open("input.txt", [:read], fn file ->
  {:ok, line} = IO.read(file, :line)
  a = line |> String.split |> Enum.map(String.to_integer)
  IO.puts(a)
end)
