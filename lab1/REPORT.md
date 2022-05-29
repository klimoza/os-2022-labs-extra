# Лабораторная работа № 3
Автор: Климов Иван, 20.Б10-мкн

### **Задание 1**
С материалами ознакомился.

### **Задание 2**
Посмотрим какие еще инструкции нам выдадут:
```console
[f000:e05b]    0xfe05b:	cmpl   $0x0,%cs:0x6c48
[f000:e062]    0xfe062:	jne    0xfd2e1
[f000:e066]    0xfe066:	xor    %dx,%dx
[f000:e068]    0xfe068:	mov    %dx,%ss
[f000:e06a]    0xfe06a:	mov    $0x7000,%esp
[f000:e070]    0xfe070:	mov    $0xf3691,%edx
```
Первые две инструкции делают какие-то сравнения, видимо идет проверка каких-то флагов. В следующих двух командах настраиваются регистр данных ```DX``` и сегмент стека ```SS```, они оба присваиваются нулями. Дальше продолжается настройка стека, инициализация каких-то полей какими-то значениями.

### **Задание 3**

- Процессор начинает выполнять 32-битные команд после выполнения следующей инструкции:
  ```console
      # Jump to next instruction, but in 32-bit code segment.
      # Switches processor into 32-bit mode.
      ljmp    $PROT_MODE_CSEG, $protcseg
  ```

- Для того что-бы узнать какая инструкция bootloader-а была выполнена последней заглянем в файл ```obj/boot/boot.asm```:
  ```console
  	// call the entry point from the ELF header
  	((void (*)(void)) (ELFHDR->e_entry))();
      7d6b:	ff 15 18 00 01 00    	call   *0x10018
  ```
  Соответственно чтобы узнать какая инструкция была первой загружена в ядре, нужно лишь узнать что произошло после ```0x7d6b```. Это можно сделать с помощью ```gdb```: 
  ```console
  (gdb) b *0x7d6b
  Breakpoint 1 at 0x7d6b
  (gdb) c
  Continuing.
  The target architecture is assumed to be i386
  => 0x7d6b:	call   *0x10018

  Breakpoint 1, 0x00007d6b in ?? ()
  (gdb) si
  => 0x10000c:	movw   $0x1234,0x472
  ```

- Инструкция лежит в файле ```kern/entry.S```:
  ```console
  .globl entry
  entry:
  	movw	$0x1234,0x472			# warm boot
  ```

- Вся необходимая информация лежит в соответствующих ```elf```-файлах:
  ```
  obj/kern/kernel:     file format elf32-i386

  Disassembly of section .text:
  ```
  Загрузчик считывает заголовок и находит в нем нужную информацию об адресах сегментов(и, соответственно, об их количестве).

### **Задание 4**
У нас был курс C, с указателями знаком очень хорошо.

### **Задание 5**
Утверждается, что сломается следующая инструкция из файла ```boot/boot.S```:
```console
  # Switch from real to protected mode, using a bootstrap GDT
  # and segment translation that makes virtual addresses 
  # identical to their physical addresses, so that the 
  # effective memory map does not change during the switch.
  lgdt    gdtdesc
```
Действительно, инструкция считывает глобальную таблицу дескрипторов(```GDT``` как указано в описании), а мы при этом мы говорим ей неверный адрес.

### **Задание 6**
Проделаем нужные операции:
```console
(gdb) b *0x7c00
Breakpoint 1 at 0x7c00
(gdb) c
Continuing.
[   0:7c00] => 0x7c00:	cli

Breakpoint 1, 0x00007c00 in ?? ()
(gdb) x/8x 0x00100000
0x100000:	0x00000000	0x00000000	0x00000000	0x00000000
0x100010:	0x00000000	0x00000000	0x00000000	0x00000000
(gdb) b *0x10000c
Breakpoint 2 at 0x10000c
(gdb) c
Continuing.
The target architecture is assumed to be i386
=> 0x10000c:	movw   $0x1234,0x472

Breakpoint 2, 0x0010000c in ?? ()
(gdb) x/8x 0x00100000
0x100000:	0x1badb002	0x00000000	0xe4524ffe	0x7205c766
0x100010:	0x34000004	0x0000b812	0x220f0011	0xc0200fd8
```
Получили что вначале память заполнена нулями, а потом в ней лежит какая-то информация, видимо код ядра загруженный ```bootloader```-ом.

### **Задание 7**
Все сломается после следующих инструкций:
```
# Now paging is enabled, but we're still running at a low EIP
# (why is this okay?).  Jump up above KERNBASE before entering
# C code.
mov	$relocated, %eax
jmp	*%eax
```
Если присваивание пройдет еще безболезненно, то прыжок будет совершаться по непонятно с чем слинкованному адресу. Это, очевидно, очень плохо.

### **Задание 8**
Добавил(а точнее заменил неправильную обработку) в файл ```lib/printfmt.c``` следующий код:
```C
// (unsigned) octal
case 'o':
	num = getuint(&ap, lflag);
	base = 8;
	goto number;
```

### **Задание 9**
Стек инициализируется следующим образом:
```
.data
###################################################################
# boot stack
###################################################################
	.p2align	PGSHIFT		# force page alignment
	.globl		bootstack
bootstack:
	.space		KSTKSIZE
	.globl		bootstacktop   
bootstacktop:
```
А для того чтобы узнать где стек находится нам следует посмотреть на вызов следующей инструкции:
```
# Set the stack pointer
movl	$(bootstacktop),%esp
```
В файле ```obj/kern/kern.asm``` видим следующее:
```
	# Set the stack pointer
	movl	$(bootstacktop),%esp
f0100034:	bc 00 00 11 f0       	mov    $0xf0110000,%esp
```
Соответственно стек будет находится по адресу ```0xf0110000``` и указывать этот адрес будет на верхний конец(самый верхний адрес) стека.

### **Задание 10**
Посмотрим в ```obj/kern/kernel.asm```:
```
f0100040 <test_backtrace>:
#include <kern/console.h>

// Test the stack backtrace function (lab 1 only)
void
test_backtrace(int x)
{
f0100040:	55                   	push   %ebp
f0100041:	89 e5                	mov    %esp,%ebp
f0100043:	53                   	push   %ebx
f0100044:	83 ec 0c             	sub    $0xc,%esp
f0100047:	8b 5d 08             	mov    0x8(%ebp),%ebx
	cprintf("entering test_backtrace %d\n", x);
f010004a:	53                   	push   %ebx
f010004b:	68 00 1a 10 f0       	push   $0xf0101a00
f0100050:	e8 ac 08 00 00       	call   f0100901 <cprintf>
	if (x > 0)
f0100055:	83 c4 10             	add    $0x10,%esp
f0100058:	85 db                	test   %ebx,%ebx
f010005a:	7e 11                	jle    f010006d <test_backtrace+0x2d>
		test_backtrace(x-1);
f010005c:	83 ec 0c             	sub    $0xc,%esp
f010005f:	8d 43 ff             	lea    -0x1(%ebx),%eax
f0100062:	50                   	push   %eax
f0100063:	e8 d8 ff ff ff       	call   f0100040 <test_backtrace>
f0100068:	83 c4 10             	add    $0x10,%esp
f010006b:	eb 11                	jmp    f010007e <test_backtrace+0x3e>
	else
		mon_backtrace(0, 0, 0);
f010006d:	83 ec 04             	sub    $0x4,%esp
f0100070:	6a 00                	push   $0x0
f0100072:	6a 00                	push   $0x0
f0100074:	6a 00                	push   $0x0
f0100076:	e8 f5 06 00 00       	call   f0100770 <mon_backtrace>
f010007b:	83 c4 10             	add    $0x10,%esp
	cprintf("leaving test_backtrace %d\n", x);
f010007e:	83 ec 08             	sub    $0x8,%esp
f0100081:	53                   	push   %ebx
f0100082:	68 1c 1a 10 f0       	push   $0xf0101a1c
f0100087:	e8 75 08 00 00       	call   f0100901 <cprintf>
}
f010008c:	83 c4 10             	add    $0x10,%esp
f010008f:	8b 5d fc             	mov    -0x4(%ebp),%ebx
f0100092:	c9                   	leave  
f0100093:	c3                   	ret    
```
При рекурсивном вызове на стек кладется сохраненное значение ```x```(регистр ```eax```), указатель на начало стека ```ebp``` и регистр ```ebx```. Соответсвенно на последней итерации рекурсивного вызова не происходит, соответсвенно нам не нужно сохранять ```x```, но вместо этого нужно сохранить 3 параметра для функции ```mon_backtrace```.

### **Задание 11**
Функция ```mon_backtrace``` реализована в ```kern/monitor.c```.