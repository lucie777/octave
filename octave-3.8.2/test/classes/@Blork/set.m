function s = set (s, varargin)

  propArgs = varargin;
  while (length (propArgs) >= 2)
    propName  = propArgs{1};
    propValue = propArgs{2};
    propArgs  = propArgs(3:end);
    switch propName
      case 'bleek'
        s.bleek = propValue;
      otherwise
        error ([propName, ' is not a valid Blork property']);
    end
  end

end
