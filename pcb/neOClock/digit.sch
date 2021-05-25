EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Display_Character:KCSC02-136 U2
U 1 1 61047CC9
P 5995 2810
F 0 "U2" H 5995 3477 50  0000 C CNN
F 1 "KCSC02-136" H 5995 3386 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 5995 2210 50  0001 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-136(Ver.6B).pdf" H 5495 3285 50  0001 L CNN
	1    5995 2810
	0    -1   -1   0   
$EndComp
$Comp
L Display_Character:KCSC02-136 U9
U 1 1 61047CCF
P 5995 4310
F 0 "U9" H 5995 4977 50  0000 C CNN
F 1 "KCSC02-136" H 5995 4886 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 5995 3710 50  0001 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-136(Ver.6B).pdf" H 5495 4785 50  0001 L CNN
	1    5995 4310
	0    -1   1    0   
$EndComp
$Comp
L Device:LED D5
U 1 1 61047CDB
P 9300 3040
F 0 "D5" V 9247 3118 50  0000 L CNN
F 1 "LED" V 9338 3118 50  0000 L CNN
F 2 "LED_THT:LED_D5.0mm" H 9300 3040 50  0001 C CNN
F 3 "~" H 9300 3040 50  0001 C CNN
	1    9300 3040
	0    1    1    0   
$EndComp
$Comp
L Device:LED D7
U 1 1 61047CE1
P 9800 3040
F 0 "D7" V 9839 2923 50  0000 R CNN
F 1 "LED" V 9748 2923 50  0000 R CNN
F 2 "LED_THT:LED_D5.0mm" H 9800 3040 50  0001 C CNN
F 3 "~" H 9800 3040 50  0001 C CNN
	1    9800 3040
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5695 3110 5695 3535
$Comp
L Display_Character:KCSC02-136 U11
U 1 1 61047CE8
P 8405 4310
F 0 "U11" H 8405 4977 50  0000 C CNN
F 1 "KCSC02-136" H 8405 4886 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 8405 3710 50  0001 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-136(Ver.6B).pdf" H 7905 4785 50  0001 L CNN
	1    8405 4310
	0    -1   1    0   
$EndComp
$Comp
L Display_Character:KCSC02-136 U10
U 1 1 61047CEE
P 8405 2810
F 0 "U10" H 8405 3477 50  0000 C CNN
F 1 "KCSC02-136" H 8405 3386 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 8405 2210 50  0001 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-136(Ver.6B).pdf" H 7905 3285 50  0001 L CNN
	1    8405 2810
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8105 3110 8105 3585
Wire Wire Line
	8205 4010 8205 3650
Wire Wire Line
	8305 3110 8305 3710
Wire Wire Line
	8405 4010 8405 3765
Wire Wire Line
	8505 3110 8505 3825
Wire Wire Line
	8605 4010 8605 3485
Wire Wire Line
	8705 3110 8705 3165
Wire Wire Line
	5795 3110 5795 3485
Wire Wire Line
	5895 4010 5895 3710
Wire Wire Line
	5995 3110 5995 3390
Wire Wire Line
	6095 4010 6095 3825
Wire Wire Line
	6195 3110 6195 3300
Wire Wire Line
	6295 4010 6295 3345
Wire Wire Line
	3835 2055 3835 1840
Wire Wire Line
	3835 1840 5170 1840
Wire Wire Line
	5170 1840 5170 2510
Wire Wire Line
	5170 3165 8705 3165
Connection ~ 8705 3165
Wire Wire Line
	8705 3165 8705 3405
Wire Wire Line
	6295 2510 5170 2510
Connection ~ 5170 2510
Wire Wire Line
	5170 2510 5170 3165
Wire Wire Line
	3735 2055 3735 1890
Wire Wire Line
	3735 1890 5115 1890
Wire Wire Line
	5115 1890 5115 3210
Wire Wire Line
	5115 3210 8605 3210
Connection ~ 8605 3210
Wire Wire Line
	8605 3210 8605 3110
Wire Wire Line
	5115 3210 5115 4610
Wire Wire Line
	5115 4610 6295 4610
Connection ~ 5115 3210
Wire Wire Line
	3635 2055 3635 1940
Wire Wire Line
	3635 1940 5065 1940
Wire Wire Line
	5065 1940 5065 3255
Wire Wire Line
	5065 3255 6295 3255
Connection ~ 6295 3255
Wire Wire Line
	6295 3255 6295 3110
Wire Wire Line
	3535 2055 3535 1990
Wire Wire Line
	3535 1990 5010 1990
Wire Wire Line
	5010 1990 5010 3300
Wire Wire Line
	5010 3300 6195 3300
Connection ~ 6195 3300
Wire Wire Line
	6195 3300 6195 3890
Wire Wire Line
	3635 5055 3635 5135
Wire Wire Line
	3635 5135 5010 5135
Wire Wire Line
	5010 5135 5010 3345
Wire Wire Line
	5010 3345 6095 3345
Connection ~ 6095 3345
Wire Wire Line
	6095 3345 6095 3110
Wire Wire Line
	3735 5055 3735 5195
Wire Wire Line
	3735 5195 5065 5195
Wire Wire Line
	5065 5195 5065 3390
Wire Wire Line
	5065 3390 5995 3390
Connection ~ 5995 3390
Wire Wire Line
	5995 3390 5995 3765
Wire Wire Line
	3835 5055 3835 5250
Wire Wire Line
	3835 5250 5155 5250
Wire Wire Line
	5155 5250 5155 3435
Wire Wire Line
	5155 3435 5895 3435
Connection ~ 5895 3435
Wire Wire Line
	5895 3435 5895 3110
Wire Wire Line
	3935 5055 3935 5310
Wire Wire Line
	3935 5310 5205 5310
Wire Wire Line
	5205 5310 5205 3485
Wire Wire Line
	5205 3485 5795 3485
Connection ~ 5795 3485
Wire Wire Line
	5795 3485 5795 3650
Wire Wire Line
	4035 5055 4035 5370
Wire Wire Line
	4035 5370 5260 5370
Wire Wire Line
	5260 5370 5260 3535
Wire Wire Line
	5260 3535 5695 3535
Connection ~ 5695 3535
Wire Wire Line
	5695 3535 5695 3585
Wire Wire Line
	5695 3585 8105 3585
Connection ~ 5695 3585
Wire Wire Line
	5695 3585 5695 4010
Connection ~ 8105 3585
Wire Wire Line
	8105 3585 8105 4010
Wire Wire Line
	5795 3650 8205 3650
Connection ~ 5795 3650
Wire Wire Line
	5795 3650 5795 4010
Connection ~ 8205 3650
Wire Wire Line
	8205 3650 8205 3110
Wire Wire Line
	5895 3710 8305 3710
Connection ~ 5895 3710
Wire Wire Line
	5895 3710 5895 3435
Connection ~ 8305 3710
Wire Wire Line
	8305 3710 8305 4010
Wire Wire Line
	5995 3765 8405 3765
Connection ~ 5995 3765
Wire Wire Line
	5995 3765 5995 4010
Connection ~ 8405 3765
Wire Wire Line
	8405 3765 8405 3110
Wire Wire Line
	6095 3825 8505 3825
Connection ~ 6095 3825
Wire Wire Line
	6095 3825 6095 3345
Connection ~ 8505 3825
Wire Wire Line
	8505 3825 8505 4010
Wire Wire Line
	6395 4010 6395 3960
Wire Wire Line
	6395 3960 8805 3960
Wire Wire Line
	8805 3960 8805 4010
Wire Wire Line
	8805 3110 8805 3255
Wire Wire Line
	8805 3255 6395 3255
Wire Wire Line
	6395 3255 6395 3110
Wire Wire Line
	8805 3255 8805 3960
Connection ~ 8805 3255
Connection ~ 8805 3960
Wire Wire Line
	3935 2055 3935 2020
Wire Wire Line
	3935 2020 4965 2020
Wire Wire Line
	4965 2020 4965 3960
Wire Wire Line
	4965 3960 6395 3960
Connection ~ 6395 3960
Wire Wire Line
	6195 3890 7675 3890
Wire Wire Line
	7675 3890 7675 4610
Wire Wire Line
	7675 4610 8705 4610
Connection ~ 6195 3890
Wire Wire Line
	6195 3890 6195 4010
Wire Wire Line
	6295 3345 7620 3345
Wire Wire Line
	7620 3345 7620 2510
Wire Wire Line
	7620 2510 8705 2510
Connection ~ 6295 3345
Wire Wire Line
	6295 3345 6295 3255
Connection ~ 7620 3345
Wire Wire Line
	9805 4250 9805 4210
Wire Wire Line
	9305 4250 9805 4250
Wire Wire Line
	9305 4210 9305 4250
Wire Wire Line
	7620 3345 9305 3345
$Comp
L Device:LED D6
U 1 1 61047D73
P 9305 4060
F 0 "D6" V 9252 4138 50  0000 L CNN
F 1 "LED" V 9343 4138 50  0000 L CNN
F 2 "LED_THT:LED_D5.0mm" H 9305 4060 50  0001 C CNN
F 3 "~" H 9305 4060 50  0001 C CNN
	1    9305 4060
	0    1    1    0   
$EndComp
$Comp
L Device:LED D8
U 1 1 61047D79
P 9805 4060
F 0 "D8" V 9844 3943 50  0000 R CNN
F 1 "LED" V 9753 3943 50  0000 R CNN
F 2 "LED_THT:LED_D5.0mm" H 9805 4060 50  0001 C CNN
F 3 "~" H 9805 4060 50  0001 C CNN
	1    9805 4060
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9305 3910 9305 3795
Wire Wire Line
	9305 3795 9805 3795
Wire Wire Line
	9805 3795 9805 3910
Connection ~ 9305 3795
Wire Wire Line
	9305 3795 9305 3345
Wire Wire Line
	8705 3405 9125 3405
Wire Wire Line
	9125 3405 9125 2745
Wire Wire Line
	9125 2745 9300 2745
Wire Wire Line
	9300 2745 9300 2890
Connection ~ 8705 3405
Wire Wire Line
	8705 3405 8705 4010
Wire Wire Line
	9245 3485 9245 3230
Wire Wire Line
	9245 3230 9300 3230
Wire Wire Line
	9300 3230 9300 3190
Wire Wire Line
	9300 2745 9800 2745
Wire Wire Line
	9800 2745 9800 2890
Connection ~ 9300 2745
Wire Wire Line
	9300 3230 9800 3230
Wire Wire Line
	9800 3230 9800 3190
Connection ~ 9300 3230
Wire Wire Line
	8605 3485 9245 3485
Connection ~ 8605 3485
Wire Wire Line
	8605 3485 8605 3210
Wire Wire Line
	4035 2055 4035 1750
Wire Wire Line
	4135 2055 4135 1675
Wire Wire Line
	6295 2510 6395 2510
Connection ~ 6295 2510
Wire Wire Line
	8705 2510 8805 2510
Connection ~ 8705 2510
Wire Wire Line
	9805 4250 9805 4610
Wire Wire Line
	9805 4610 8805 4610
Connection ~ 9805 4250
Wire Wire Line
	6295 4610 6395 4610
Connection ~ 6295 4610
Wire Wire Line
	3435 2055 3435 2015
Wire Wire Line
	2040 2300 2980 2300
Wire Wire Line
	2980 2300 2980 5100
Wire Wire Line
	2980 5100 3435 5100
Wire Wire Line
	3435 5100 3435 5055
Wire Wire Line
	3535 5055 3535 5130
Wire Wire Line
	3535 5130 2865 5130
Wire Wire Line
	2865 5130 2865 2400
Wire Wire Line
	2865 2400 2040 2400
Wire Wire Line
	2715 5600 4135 5600
Wire Wire Line
	4135 5600 4135 5055
Text HLabel 2055 1675 0    50   Output ~ 0
Sda
Text HLabel 2050 1750 0    50   Output ~ 0
Scl
Text HLabel 2040 2300 0    50   Output ~ 0
Input1
Text HLabel 2040 2400 0    50   Output ~ 0
Input2
Text HLabel 2045 2500 0    50   Output ~ 0
V+
Wire Wire Line
	2045 2500 2715 2500
Wire Wire Line
	2715 2500 2715 5600
Wire Wire Line
	4035 1750 2050 1750
Wire Wire Line
	4135 1675 2055 1675
Wire Wire Line
	2060 2015 3435 2015
Text HLabel 2060 2015 0    50   Output ~ 0
Gnd
$Comp
L MAX6959:MAX6959AAEE+ U?
U 1 1 60A8CF64
P 4135 2055
F 0 "U?" V 5582 2283 60  0000 L CNN
F 1 "MAX6959AAEE+" V 5688 2283 60  0000 L CNN
F 2 "21-0055H_16" H 5635 2295 60  0001 C CNN
F 3 "" H 4135 2055 60  0000 C CNN
	1    4135 2055
	0    1    1    0   
$EndComp
$EndSCHEMATC