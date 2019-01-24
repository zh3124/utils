# BM模式匹配算法

## 关键规则

模式串向右移动，移动字符数是通过两条规则决定的：坏字符规则和好后缀规则。实际移动为通过这两条规则计算出的最大移动个数。

**T表示主串 P表示模式串**

- 坏字符规则，坏字符即从后往前匹配，主串中第一个与模式串不匹配的字符

    
    移动原则：
    
    当T中有字符不匹配时，如果T中的这个不匹配的字符出现在对应P中当前位置的左侧，那么P移动位置将这两个在字符对齐。如果T中这个不匹配字符不在P中当前位置的左侧，那么将当前位置左侧的所有字符均移到该不匹配字符后。

- 好后缀规则，好后缀即从后往前匹配，模式串中与主串能匹配的后缀字符串

    移动原则：
    
    - 1)当T中有字符不匹配时且此时P与T中尾缀相同为p且若len(p)>0(即有相同尾缀);
    
    - 2)此时在模式串P中,p的左侧(P->P[len(P)-1])从右往左寻找第一个与p相同的p';

        - 2.1)若p'存在，则模式串P向右移动至p'与p对齐;

        - 2.2)若p'不存在，则寻找比p短一位的尾缀p1，若p1长度不为0，则执行步骤2)若为0，则执行步骤3);

    - 3)向右移动P到P之后（向右移动len(P)）;


## DEMO查找过程

- 1.主串与模式串头部对齐，从尾部开始比较。如果尾部字符不匹配(<font color=red>"S"</font>与<font color=red>"E"</font>不匹配),<font color=red>"S"</font>就被称为"坏字符"

    "S"的坏字符规则，模式串向右移动7个字符

    此时没有好尾缀

H|E|R|E|&nbsp;|I|<font color=red>S</font>|&nbsp;|A|&nbsp;|S|I|M|P|L|E|&nbsp;|E|X|A|M|P|L|E
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
E|X|A|M|P|L|E

- 2.此时<font color=red>"P"</font>与<font color=red>"E"</font>不匹配，<font color=red>"P"</font>为坏字符

    "P"的坏字符规则，模式串向右移动2个字符，使模式串中前一个P与主串中P对齐

    此时没有好尾缀

H|E|R|E|&nbsp;|I|S|&nbsp;|A|&nbsp;|S|I|M|<font color=red>P</font>|L|E|&nbsp;|E|X|A|M|P|L|E
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|E|X|A|M|P|L|E


- 3.此时<font color=red>"I"</font>与<font color=red>"A"</font>不匹配，<font color=red>"I"</font>为坏字符, <font color=green>"MPLE"</font>为好后缀

    "I"的坏字符规则，模式串向右移动3个字符

    "MPLE"好后缀，模式串前未找到

    "PLE"后缀，模式串前未找到

    "LE"后缀，模式串前未找到

    "E"后缀，模式串找到，两个E之间相差6个字符
    
    因此，"MPLE"的好后缀规则，模式串向右移动6个字符

    综上，向右移动6个字符
    

H|E|R|E|&nbsp;|I|S|&nbsp;|A|&nbsp;|S|<font color=red>I</font>|M|P|L|E|&nbsp;|E|X|A|M|P|L|E
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|E|X|A|<font color=green>M</font>|<font color=green>P</font>|<font color=green>L</font>|<font color=green>E</font>

- 4.<font color=red>"P"</font>为坏字符

    "P"的坏字符规则，向右移动2个字符

H|E|R|E|&nbsp;|I|S|&nbsp;|A|&nbsp;|S|I|M|P|L|E|&nbsp;|E|X|A|M|<font color=red>P</font>|L|E
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|E|X|A|M|P|L|E

- 5.成功匹配

H|E|R|E|&nbsp;|I|S|&nbsp;|A|&nbsp;|S|I|M|P|L|E|&nbsp;|E|X|A|M|P|L|E
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|&nbsp;|<font color=green>E</font>|<font color=green>X</font>|<font color=green>A</font>|<font color=green>M</font>|<font color=green>P</font>|<font color=green>L</font>|<font color=green>E</font>


