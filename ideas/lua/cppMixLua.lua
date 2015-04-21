function Test(i)
    print("lua Test()", i)
    print_test(i)

    t = CppTest(i)
    t:print()
    print("CppTest._i = ",t._i)
    return i + 3
end

