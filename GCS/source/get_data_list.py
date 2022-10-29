def parse_string(input_list):
    #Telemetry format: 
    #
    res = input_list.split(",")
    return res


if __name__ == '__main__':
    sample = "2022ASI-005,0022,0022,2200.1,101.3,27.3,3.5,030742.00,02232.73830,11404.58520,32908.09,12,10,2145,1,3.4"
    print(parse_string(sample))
	