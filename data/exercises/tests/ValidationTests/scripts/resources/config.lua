-- Config_

config =
{
	positions =
	{
		destination =
		{
			"31TDP5864614485",
		},

		center_All =
		{
			"30TXT7537085669",
		},

		center_ABC=
		{
			"30TXU3524407672",
			"30TXU9649709845",
			"30TXT3794134194",
			"30TXT9912736367",
			"31UCP1866527133",
		},

		center_INF=
		{
			"30UXU5183528099",
			"30UYU1300130400",
			"30TXT5448754582",
			"30TYT1569256828",
			"31UCP1866527133",
		},

		center_ASS=
		{
			"30UXU6424038190",
			"31UBP7930940344",
			"30TXT6684564776",
			"31TBN7631266692",
		},
	},

}

formation =
{
-- Les Gentils :
	{
		id = 10,
		name = "Bataillon Case 1",
		automat =
		{
			{
				id = 13,
				name = "Compagnie INF AMX 10 P",
				agent =
				{
				--formation[1].automat[1]
					{
					id = 14,
					posinit = "30TXU5776110907",
					},
					{
					id = 15,
					posinit = "30UXU5283419295",
					},
					{
					id = 16,
					posinit = "30TXU5520214460",
					},
					{
					id = 17,
					posinit = "30TXU5817411214",
					},
					{
					id = 18,
					posinit = "30TXU5751811352",
					},
					{
					id = 133,
					posinit = "30TXU5598914884",
					},
				},
			},
			{
			--formation[1].automat[2]
				id = 37,
				name = "Escadron de Chars LECLERC",
				agent =
				{
					{
					id = 38,
					posinit = "30TXU4018000412",
					},
					{
					id = 39,--formation[1].automat[2].agent[2].id
					posinit = "30TXU3437505709",
					},
					{
					id = 40,
					posinit = "30TXT4021699413",
					},
					{
					id = 41,
					posinit = "30TXT3969799895",
					},
					{
					id = 42,
					posinit = "30TXU4068000429",
					},
				},
			},
			{
			--formation[1].automat[3]
				id = 61,
				name = "Escadron ERIAC",
				agent =
				{
					{
					id = 62,
					posinit = "30TXT3846199744",
					},
					{
					id = 63, --formation[1].automat[3].agent[2].id
					posinit = "30TXT3827899943",
					},
					{
					id = 64,
					posinit = "30TXU3759000270",
					},
					{
					id = 65,
					posinit = "30TXU3754700649",
					},
					{
					id = 66,
					posinit = "30TXU3767000507",
					},
				},
			},
		},
	},
	{
		id = 5,
		name = "Bataillon Case 2",
		automat =
		{
			{
			--formation[2].automat[1]
				id = 19,
				name = "Compagnie INF AMX 10 P",
				agent =
				{
					{
					id = 20,
					posinit = "30TYU1822714081",
					},
					{
					id = 21,
					posinit = "30UYU1334321676",
					},
					{
					id = 22,
					posinit = "30TYU1574417481",
					},
					{
					id = 23,
					posinit = "30TYU1926213372",
					},
					{
					id = 24,
					posinit = "30TYU1876814425",
					},
					{
					id = 134,
					posinit = "30TYU1536918147",
					},
				},
			},
			{
			--formation[2].automat[2]
				id = 43,
				name = "Escadron de Chars LECLERC",
				agent =
				{
					{
					id = 44,
					posinit = "30TYU0136602640",
					},
					{
					id = 45, --formation[2].automat[2].agent[2].id
					posinit = "30TXU9565208031",
					},
					{
					id = 46,
					posinit = "30TYU0140201641",
					},
					{
					id = 47,
					posinit = "30TYU0088302123",
					},
					{
					id = 48,
					posinit = "30TYU0186702658",
					},
				},
			},
			{
			--formation[2].automat[3]
				id = 67,
				name = "Escadron ERIAC",
				agent =
				{
					{
					id = 68,
					posinit = "30TYU0068400718",
					},
					{
					id = 69, --formation[2].automat[3].agent[2].id
					posinit = "30TYU0069300454",
					},
					{
					id = 70,
					posinit = "30TYU0096300318",
					},
					{
					id = 71,
					posinit = "30TYU0148200600",
					},
					{
					id = 72,
					posinit = "30TYU0127300680",
					},
				},
			},
		},
	},
	{
		id = 7,
		name = "Bataillon Case 3",
		automat =
		{
			{
			--formation[3].automat[1]
				id = 25,
				name = "Compagnie INF AMX 10 P",
				agent =
				{
					{
					id = 26,
					posinit = "30TXT5984137163",
					},
					{
					id = 27,
					posinit = "30TXT5485545958",
					},
					{
					id = 28,
					posinit = "30TXT5437245441",
					},
					{
					id = 29,
					posinit = "30TXT5800340884",
					},
					{
					id = 30,
					posinit = "30TXT6057337644",
					},
					{
					id = 135,
					posinit = "30TXT5737641678",
					},
				},
			},
			{
			--formation[3].automat[2]
				id = 49,
				name = "Escadron de Chars LECLERC",
				agent =
				{
					{
					id = 50,
					posinit = "30TXT4282326949",
					},
					{
					id = 51, --formation[3].automat[2].agent[2].id
					posinit = "30TXT3704732127",
					},
					{
					id = 52,
					posinit = "30TXT4285825951",
					},
					{
					id = 53,
					posinit = "30TXT3739931815",
					},
					{
					id = 54,
					posinit = "30TXT4332326967",
					},
				},
			},
			{
			--formation[3].automat[3]
				id = 73,
				name = "Escadron ERIAC",
				agent =
				{
					{
					id = 74,
					posinit = "30TXT4268925735",
					},
					{
					id = 75,--formation[3].automat[3].agent[2].id
					posinit = "30TXT4249725611",
					},
					{
					id = 76,
					posinit = "30TXT4204225751",
					},
					{
					id = 77,
					posinit = "30TXT4158925813",
					},
					{
					id = 78,
					posinit = "30TXT4111325952",
					},
				},
			},
		},
	},
	{
		id = 9,
		name = "Bataillon Case 4",
		automat =
		{
			{
			--formation[4].automat[1]
				id = 31,
				name = "Compagnie INF AMX 10 P",
				agent =
				{
					{
					id = 32,
					posinit = "30TYT2136839799",
					},
					{
					id = 33,
					posinit = "30TYT1608248108",
					},
					{
					id = 34,
					posinit = "30TYT1559947591",
					},
					{
					id = 35,
					posinit = "30TYT1508048072",
					},
					{
					id = 36,
					posinit = "30TYT2076740816",
					},
					{
					id = 136,
					posinit = "30TYT1796444718",
					},
				},
			},
			{
			--formation[4].automat[2]
				id = 55,
				name = "Escadron de Chars LECLERC",
				agent =
				{
					{
					id = 56,
					posinit = "30TYT0400829138",
					},
					{
					id = 57, --formation[4].automat[2].agent[2].id
					posinit = "30TXT9821934437",
					},
					{
					id = 58,
					posinit = "30TXT9819534553",
					},
					{
					id = 59,
					posinit = "30TYT0352528621",
					},
					{
					id = 60,
					posinit = "30TYT0450929155",
					},
				},
			},
			{
			--formation[4].automat[3]
				id = 79,
				name = "Escadron ERIAC",
				agent =
				{
					{
					id = 80,
					posinit = "30TYT0268026994",
					},
					{
					id = 81,--formation[4].automat[3].agent[2].id
					posinit = "30TYT0333627272",
					},
					{
					id = 82,
					posinit = "30TYT0369227187",
					},
					{
					id = 83,
					posinit = "30TYT0317026973",
					},
					{
					id = 84,
					posinit = "30TYT0295827434",
					},
				},
			},
		},
	},
-- Les Méchants :
	{
		id = 11,
		name = "GTIA Case 1",
		automat =
		{
			{
				id = 85,
				name = "Escadron de Chars AMX30B2 BRENNUS",
				agent =
				{
				--formation[5].automat[1]
					{
					id = 90,
					posinit = "30TXU2654015282",
					},
					{
					id = 86,
					posinit = "30UXU5283419295",
					},
				},
			},
			{
				id = 109,
				name = "Compagnie INF VAB",
				agent =
				{
				--formation[5].automat[2]
					{
					id = 110,
					posinit = "30UXU5338145970",
					},
					{
					id = 111,
					posinit = "30UXU5188437317",
					},
					{
					id = 112,
					posinit = "30UXU5260643108",
					},
					{
					id = 113,
					posinit = "30UXU5332845644",
					},
					{
					id = 114,
					posinit = "30UXU5340346263",
					},
				},
			},
		},
	},
	{
		id = 12,
		name = "GTIA Case 2",
		automat =
		{
			{
				id = 91,
				name = "Escadron de Chars AMX30B2 BRENNUS",
				agent =
				{
				--formation[6].automat[1]
					{
					id = 94,
					posinit = "30TXU8817617329",
					},
					{
					id = 95,
					posinit = "30UXU8052322418",
					},
					{
					id = 96,
					posinit = "30TXU8780618153",
					},
					{
					id = 92,
					posinit = "30UXU8082723325",
					},
					{
					id = 93,
					posinit = "30UXU8017022803",
					},
				},
			},
			{
				id = 115,
				name = "Compagnie INF VAB",
				agent =
				{
				--formation[6].automat[2]
					{
					id = 117,
					posinit = "30UYU1307439546",
					},
					{
					id = 118,
					posinit = "30UYU1259239029",
					},
					{
					id = 119,
					posinit = "30UYU1414046535",
					},
					{
					id = 120,
					posinit = "30UYU1469048856",
					},
					{
					id = 116,
					posinit = "30UYU1453348688",
					},
				},
			},
		},
	},
	{
		id = 6,
		name = "GTIA Case 3",
		automat =
		{
			{
				id = 97,
				name = "Escadron de Chars AMX30B2 BRENNUS",
				agent =
				{
				--formation[7].automat[1]
					{
					id = 100,
					posinit = "30TXT2992140822",
					},
					{
					id = 101,
					posinit = "30TXT2256346801",
					},
					{
					id = 102,
					posinit = "30TXT2971440844",
					},
					{
					id = 98,
					posinit = "30TXT2227346703",
					},
					{
					id = 99,
					posinit = "30TXT2279147014",
					},
				},
			},
			{
				id = 121,
				name = "Compagnie INF VAB",
				agent =
				{
				--formation[7].automat[2]
					{
					id = 123,
					posinit = "30TXT5451663852",
					},
					{
					id = 124,
					posinit = "30TXT5508368910",
					},
					{
					id = 125,
					posinit = "30TXT5618873099",
					},
					{
					id = 126,
					posinit = "30TXT5615672795",
					},
					{
					id = 122,
					posinit = "30TXT5603672488",
					},
				},
			},
		},
	},{
		id = 8,
		name = "GTIA Case 4",
		automat =
		{
			{
				id = 103,
				name = "Escadron de Chars AMX30B2 BRENNUS",
				agent =
				{
				--formation[8].automat[1]
					{
					id = 106,
					posinit = "30TXT8454549015",
					},
					{
					id = 107,
					posinit = "30TXT8405149153",
					},
					{
					id = 108,
					posinit = "30TXT9093143127",
					},
					{
					id = 104,
					posinit = "30TXT8425748301",
					},
					{
					id = 105,
					posinit = "30TXT8393049227",
					},
				},

			},
			{
				id = 127,
				name = "Escadron de Chars LECLERC",
				agent =
				{
				--formation[8].automat[2]
					{
					id = 129,
					posinit = "30TYT1570266046",
					},
					{
					id = 130,
					posinit = "30TYT1521965529",
					},
					{
					id = 131,
					posinit = "30TYT1714874462",
					},
					{
					id = 128,
					posinit = "30TYT1746475318",
					},
					{
					id = 132,
					posinit = "30TYT1732474988",
					},
				},
			},
		},
	},
}
