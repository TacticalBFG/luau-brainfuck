# luau-brainfuck
i shouldve slept

this is basically a translator from brainfuck into Roblox's modified lua, luau.

it should work as long as the bytecode & instruction format doesnt change.

this compiles directly to roblox's bytecode format, meaning all you need to do is

1. call the translator/serializer to serialize ur script into luau
2. pass the serialized shit into the deserializer
3. call the function the deserializer returns

pretty straightforward tbh
