# tinky_winkey

## O que é este programa

O **tinky_winkey** é um projeto em C para Windows composto por dois executáveis:
- `svc.exe`: gere um serviço do Windows (instalar, iniciar, parar e remover).
- `winkey.exe`: executa um loop de mensagens e instala um hook de teclado (`WH_KEYBOARD_LL`) para capturar eventos de teclas e registar atividade em ficheiro.

O objetivo é demonstrar, em contexto académico, como diferentes APIs do Windows podem trabalhar juntas num projeto real.

## O que se aprende com este projeto

Com este projeto, podes estudar:
- Estrutura de um projeto C com múltiplos ficheiros (`srcs/` e `incs/`).
- Criação e gestão de serviços no Windows.
- Ciclo de mensagens (`GetMessage`, `TranslateMessage`, `DispatchMessage`).
- Uso de hooks de teclado de baixo nível.
- Manipulação básica de processos/tokens no ecossistema Win32.
- Organização de logging e separação de responsabilidades por módulo.

## Aviso importante

Este projeto foi criado **apenas para fins educativos**.

Não deve ser usado para atividades ilegais, maliciosas ou para prejudicar terceiros.

O autor não se responsabiliza por qualquer uso indevido deste conteúdo.