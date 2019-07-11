//	Библиотека позволяет работать с ИК-термометром на базе чипа MLX90614ESF-AAA: http://
//  Версия: 1.0.0
//  Последнюю версию библиотеки Вы можете скачать по ссылке: http://
//  Подробное описание функции бибилиотеки доступно по ссылке: http://
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_IR_Thermometer_h
#define iarduino_IR_Thermometer_h

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define	IRT_OK				0									//	Данные получены и обработаны без ошибок
#define	IRT_NO_REPLY		-1									//	Данных нет, датчик не реагирует или отсутствует
#define	IRT_CHECKSUM		-2									//	Ошибка контрольной суммы (данные прочтены не корректно, мешает другое устройство на шине или имеются помехи)

#define	IRT_code_RAM		0x00								//	код доступа к ячейкам ОЗУ	(32x17)
#define	IRT_code_ROM		0x20								//	код доступа к ячейкам ПЗУ	(32x16)
#define	IRT_code_FLG		0xF0								//	код доступа к флагам		( 1x16)
#define	IRT_code_SLP		0xFF								//	код команды перехода в спящий режим

#define	IRT_addr_IR1		0x04								//	адрес ячейки ОЗУ с исходными данными 1 ИК-датчика		(IR 1)
#define	IRT_addr_IR2		0x05								//	адрес ячейки ОЗУ с исходными данными 2 ИК-датчика		(IR 2)
#define	IRT_addr_Ta			0x06								//	адрес ячейки ОЗУ с температурой окружающей среды		(Ta)
#define	IRT_addr_To1		0x07								//	адрес ячейки ОЗУ с температурой объекта от 1 ИК-датчика	(To 1)
#define	IRT_addr_To2		0x08								//	адрес ячейки ОЗУ с температурой объекта от 2 ИК-датчика	(To 2)

#define	IRT_addr_To_max		0x00								//	адрес ячейки ПЗУ с указанием максимального значения для температуры объекта	(To)
#define	IRT_addr_To_min		0x01								//	адрес ячейки ПЗУ с указанием минимального  значения для температуры объекта	(To)
#define	IRT_addr_PWM		0x02								//	адрес ячейки ПЗУ с указанием настроек ШИМ									(PWM)
#define	IRT_addr_Ta_range	0x03								//	адрес ячейки ПЗУ с указанием настроек для температуры окружающей среды		(Ta)
#define	IRT_addr_K			0x04								//	адрес ячейки ПЗУ с указанием поправочного коэффициента						(K)
#define	IRT_addr_CONF		0x05								//	адрес ячейки ПЗУ с указанием конфигурационных данных
#define	IRT_addr_ID			0x0E								//	адрес ячейки ПЗУ с указанием ID адреса датчика на шине I2C (только последний байт)
#define	IRT_addr_SERIAL0	0x1C								//	адрес ячейки ПЗУ с указанием 1 части серийного номера датчика
#define	IRT_addr_SERIAL1	0x1D								//	адрес ячейки ПЗУ с указанием 2 части серийного номера датчика
#define	IRT_addr_SERIAL2	0x1E								//	адрес ячейки ПЗУ с указанием 3 части серийного номера датчика
#define	IRT_addr_SERIAL3	0x1F								//	адрес ячейки ПЗУ с указанием 4 части серийного номера датчика

class iarduino_IR_Thermometer{
	public:
	/**	пользовательские функции **/
		iarduino_IR_Thermometer			(uint8_t=0);			//	подключение датчика				( [адрес на шине I2C] )
		int		begin  					();						//	инициализация датчика			()
		int		read   					();						//	чтение показаний датчика		()
		int		newID					(uint8_t);				//	установка адреса на шине I2C	( адрес на шине I2C )
	   uint16_t	serial					(uint8_t);				//	получение серийного номера		( позиция )

	/**	переменные доступные для пользователя **/
		double	temp;											//	значение температуры объекта
		double	tempA;											//	значение температуры окружающей среды
		
	private:
	/**	внутренние функции **/
		uint8_t	IRT_func_CRC8(bool);							//	создание	циклически избыточного кода	CRC8			( 0-запись / 1-чтение )

	/**	внутренние переменные **/
		uint8_t	IRT_uint_ADDRESS		=	0x5A;				//	адрес		датчика на шине I2C
		uint8_t	IRT_data_COM;									//	данные		команды датчика
		uint8_t	IRT_data_MSB;									//	данные		старший байт
		uint8_t	IRT_data_LSB;									//	данные		младший байт
		uint8_t	IRT_data_CRC;									//	данные		циклически избыточный код
	   uint16_t	I2C_mass_STATUS[0x04]	=	{100,1000,1,0x78};	//	скорость	работы шины в кГц (макс F_CPU/2  ), ожидание сброса флага TWINT в циклах, флаг успешного результата, содержимое флагов TWS регистра состояния TWSR

	/**	функции для работы с шиной I2C **/
		void	I2C_func_BEGIN			();						//	установка	скорости работы шины I2C					(без параметров)
		void	I2C_func_START			();						//	выполнение	состояния START								(без параметров)
		void	I2C_func_RESTART		();						//	выполнение	состояния RESTART							(без параметров)
		void	I2C_func_STOP			();						//	выполнение	состояния STOP								(без параметров)
		void	I2C_func_SEND_ID		(uint8_t, bool);		//	передача	первого байта								(ID-адрес модуля, бит RW)
		void	I2C_func_WRITE_BYTE		(uint8_t);				//	передача	одного байта								(байт для передачи)
		uint8_t	I2C_func_READ_BYTE		(bool);					//	получение	одного байта								(бит подтверждения ACK/NACK)
		
		void	I2C_func_READ			();						//	чтение      данных из датчика
		void	I2C_func_WRITE			();						//	запись		данных в датчик
};

#endif

/*  ОПИСАНИЕ БИТОВ РЕГИСТРОВ УПРАВЛЕНИЯ:
 *  
 *  PWMctrl - регистр управления ШИМ (IRT_addr_PWM=0x02)
 *  15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
 * |                   |         | | | |MODE_PWM:
 * |                   |         | | | |0 - PWM расширенный режим
 * |                   |         | | | |1 - PWM одиночный режим
 * |                   |         | | |
 * |                   |         | | |EN_PWM:
 * |                   |         | | |0 - режим PWM отключён 
 * |                   |         | | |1 - режим PWM включён
 * |                   |         | |
 * |                   |         | |PPODB:
 * |                   |         | |0 - вывод SDA сконфигурирован как с открытым стоком (Open Drain)
 * |                   |         | |1 - вывод SDA сконфигурирован как двухтактный (Push-Pull) а скорее всего комплиментарный
 * |                   |         |
 * |                   |         |TRPWMB:
 * |                   |         |0 - Выбран режим PWM
 * |                   |         |1 - Выбран режим теплового реле
 * |                   |
 * |                   |PWM repetition number 0…62 step 2
 * |                   |
 * |   ПЕРИОД: число для рассчета периода ШИМ.
 * |   Если в этих битах записано число b0000000, то оно расценивается как 128 (b10000000)
 *  
 *  
 *  Config - конфигурационный регистр (IRT_addr_CONF=0x05)
 *  15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
 * |  |  |        |      | | |   | |IIR:
 * |  |  |        |      | | |   | |1 0 0 - 100% a1=1,        b1=0
 * |  |  |        |      | | |   | |1 0 1 - 80%  a1=0.8,      b1=0.2
 * |  |  |        |      | | |   | |1 1 0 - 67%  a1=0.666,    b1=0.333
 * |  |  |        |      | | |   | |1 1 1 - 57%  a1=0.571,    b1=0.428
 * |  |  |        |      | | |   | |0 0 0 - 50%  a1=0.5,      b1=0.5
 * |  |  |        |      | | |   | |0 0 1 - 25%  a1=0.25,     b1=0.75
 * |  |  |        |      | | |   | |0 1 0 - 17%  a1=0.166(6), b1=0.83(3)
 * |  |  |        |      | | |   | |0 1 1 - 13%  a1=0.125,    b1=0.875
 * |  |  |        |      | | |   |
 * |  |  |        |      | | |   |ПОВТОР ТЕСТА ДАТЧИКА:
 * |  |  |        |      | | |   |0 - "OFF"
 * |  |  |        |      | | |   |1 - "ON"
 * |  |  |        |      | | |
 * |  |  |        |      | | |
 * |  |  |        |      | | |0 0 - Ta, Tobj1
 * |  |  |        |      | | |0 1 - Ta, Tobj2
 * |  |  |        |      | | |1 0 - Tobj2
 * |  |  |        |      | | |1 1 - Tobj1, Tobj2
 * |  |  |        |      | |
 * |  |  |        |      | |КОЛИЧЕСТВО ИК-ДАТЧИКОВ:
 * |  |  |        |      | |0 - один
 * |  |  |        |      | |1 - два
 * |  |  |        |      |
 * |  |  |        |      |ЗНАК Ks:
 * |  |  |        |      |0 - положительный
 * |  |  |        |      |1 - отрецательный
 * |  |  |        |
 * |  |  |        |FIR:
 * |  |  |        |0 0 0 - 8  не рекомендуется
 * |  |  |        |0 0 1 - 16 не рекомендуется
 * |  |  |        |0 1 0 - 32 не рекомендуется
 * |  |  |        |0 1 1 - 64 не рекомендуется
 * |  |  |        |1 0 0 - 128
 * |  |  |        |1 0 1 - 256
 * |  |  |        |1 1 0 - 512
 * |  |  |        |1 1 1 - 1024
 * |  |  |
 * |  |  |GAIN:
 * |  |  |0  0  0 - 1 - Усилитель пропускается
 * |  |  |0  0  1 - 3
 * |  |  |0  1  0 - 6
 * |  |  |0  1  1 - 12,5
 * |  |  |1  0  0 - 25
 * |  |  |1  0  1 - 50
 * |  |  |1  1  0 - 100
 * |  |  |1  1  1 - 100
 * |  |
 * |  |ЗНАК Kt2:
 * |  |0 - положительный
 * |  |1 - отрецательный
 * |ТЕСТ ДАТЧИКА:
 * |0 - включить
 * |1 - отключить
 *  
 *  
 *  ФЛАГИ - 3 бита состояния, отправляемые в ответ на команду (IRT_code_FLG=0xF0)
 * |15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
 * |                     | | | | |Зарезервированы
 * |                     | | | |
 * |                     | | | |INIT:  - продолжается процедура инициализации POR. Низкая активность.
 * |                     | | |
 * |                     | | |EE_DEAD: - произошла повтораная ошибка EEPROM. Высокая активность.
 * |                     | |
 * |                     | |Зарезервирован
 * |                     |
 * |                     |EEBUSY       - все еще активна предыдущая команда чтения записи EEPROM
 * |
 * |Нули
 *
 *
 *  РАССЧЕТ ТЕМПЕРАТУРЫ:
 *
 *  Температура окружающей среды Ta
 *  Та[°K] = Tareg x 0,02            где Ta reg - данные из 0x06 (IRT_addr_Ta) ячейки ОЗУ (старший бит - флаг ошибки)
 *  Та[°С] = Та[°K]-273,15
 *
 *  Температура объекта To
 *  To[°K] = Toreg x 0,02            где To reg - данные из 0x07 (IRT_addr_To1), или 0x08 (IRT_addr_To2) ячейки ОЗУ (старший бит - флаг ошибки)
 *  Тo[°С] = Тo[°K]-273,15
 *
 */