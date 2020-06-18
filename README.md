# BLDC Motor Sürücü Tasarım ve Yazılımı

Bu çalışma Elektronik mühendisliği bitirme tezi olarak hazırlanmıştır.
Bu çalışma yapılırken takım arkadaşım [Ahmet Serhat AYKILIÇ](https://www.linkedin.com/in/ahmetserhataykilic/) ile birlikte yapılmıştır.

## Tanıtım

Bu çalışmada kullanılan POWER_Board ile 500V 20A (Case Temperature) için uygun güç anahtarları kullanılmıştır. 
High side tarafında bulunan güç anahtarları için IR2101 sürücü entegresi kullanılmıştır.
PCB çizilirken Faz hatları SolderMask uygulanmadı. Bu sayede Faz hatlarını güçlendirilebilinir. 
Anahtarları uygun zamanlarda iletime/yalıtıma geçirmek için STM32F103C8T6 işlemcisi kullanıldı.
3 adet Pull-Up bağlı butonlar ile başlatma yada durdurma, yön ve mod kontrolü yapılmaktadır.
Eklenen potansiyometre ile güç anahtarların Gate-Source PWM gerilimi basitçe değiştirilmektedir.
MCU kartındaki STM32F103C8T6 denetleyicisini programlama işlemi, Header'a STlink bağlanaralk yapılmaktadır.

Driver MCU

![](images/BLDC_Motor_Driver_MCU.png)

Power MCU

![](images/BLDC_Motor_Driver_Power_MCU.png)
